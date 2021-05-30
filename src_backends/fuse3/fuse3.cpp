/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of Std++.
 *
 * Std++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Std++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Std++.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include "fuse3.hpp"
//Global
#define FUSE_USE_VERSION 31
#include <cerrno>
#include <fuse.h>
//Local
#include <Std++/Containers/BinaryTreeMap/BinaryTreeMap.hpp>
#include <Std++/Multitasking/Mutex.hpp>
#include <Std++/Streams/SeekableInputStream.hpp>
#include <Std++/FileSystem/POSIXPermissions.hpp>
#include <Std++/Mathematics.hpp>
#include <Std++/FileSystem/File.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::FileSystem;

class fuseArgsCreator
{
public:
	//Constructor
	fuseArgsCreator()
	{
		this->createdArgv = nullptr;
	}

	//Destructor
	~fuseArgsCreator()
	{
		this->Free();
	}

	//Properties
	inline int argc() const
	{
		return this->args.GetNumberOfElements();
	}

	char** argv()
	{
		this->Free();

		this->createdArgv = (char**)MemAlloc(this->args.GetNumberOfElements() * sizeof(*this->createdArgv));
		for(uint32 i = 0; i < this->args.GetNumberOfElements(); i++)
		{
			const String& arg = this->args[i];
			arg.ToUTF8();
			this->createdArgv[i] = (char*)MemAlloc(arg.GetSize()+1);
			MemCopy(this->createdArgv[i], arg.GetRawZeroTerminatedData(), arg.GetSize()+1);
		}

		return this->createdArgv;
	}

	//Inline
	inline void Add(const String& string)
	{
		this->args.Push(string);
	}

private:
	DynamicArray<String> args;
	char** createdArgv;

	//Inline
	inline void Free()
	{
		if(this->createdArgv == nullptr)
			return;
		for(uint32 i = 0; i < this->args.GetNumberOfElements(); i++)
			MemFree(this->createdArgv[i]);
		MemFree(this->createdArgv);

		this->createdArgv = nullptr;
	}
};

struct StreamState
{
	UniquePointer<InputStream> inputStream;
	uint64 offset;
};

class fuse_mapper_state
{
public:
	//Constructor
	inline fuse_mapper_state(const ReadableFileSystem& fileSystem) : fileSystem(fileSystem)
	{
	}

	//Properties
	inline const ReadableFileSystem& FileSystem() const
	{
		return this->fileSystem;
	}

	//Methods
	InputStream& GetInputStream(const Path& path, uint64 offset)
	{
		this->openInputStreamsLock.Lock();
		StreamState& streamState = this->openInputStreams[path];

		if(streamState.inputStream.IsNull())
		{
			streamState.inputStream = this->fileSystem.OpenFileForReading(path, false);
			streamState.offset = 0;
		}
		InputStream* inputStream = streamState.inputStream.operator->();

		if(offset != streamState.offset)
		{
			if(streamState.inputStream.IsInstanceOf<SeekableInputStream>())
			{
				SeekableInputStream& seekableInputStream = *dynamic_cast<SeekableInputStream*>(inputStream);
				seekableInputStream.SeekTo(offset);
			}
			else if(streamState.offset < offset)
				inputStream->Skip(offset - streamState.offset);
			else
			{
				streamState.inputStream = this->fileSystem.OpenFileForReading(path, false);
				inputStream = streamState.inputStream.operator->();
				streamState.inputStream->Skip(offset);
			}
			streamState.offset = offset;
		}

		return *inputStream;
	}

	void UpdateStream(const Path& path, uint32 nBytesMovedForward)
	{
		StreamState& streamState = this->openInputStreams[path];
		streamState.offset += nBytesMovedForward;
		this->openInputStreamsLock.Unlock();
	}

private:
	//Members
	const ReadableFileSystem& fileSystem;
	BinaryTreeMap<Path, StreamState> openInputStreams;
	Mutex openInputStreamsLock;
};

static fuse_mapper_state& GetState()
{
	return *(fuse_mapper_state*) fuse_get_context()->private_data;
}

static void MapPermissions(FileType nodeType, const UniquePointer<Permissions>& permissions, struct stat *stbuf)
{
	const POSIXPermissions* posixPermissions = permissions.IsNull() ? nullptr :
			dynamic_cast<const POSIXPermissions *>(permissions.operator->());

	if(posixPermissions)
	{
		stbuf->st_gid = posixPermissions->groupId;
		stbuf->st_uid = posixPermissions->userId;

		stbuf->st_mode = posixPermissions->EncodeMode();
	}
	else
	{
		stbuf->st_gid = 0;
		stbuf->st_uid = 0;

		switch(nodeType)
		{
			case FileType::Directory:
			case FileType::Link:
				stbuf->st_mode = 0555;
				break;
			case FileType::File:
				stbuf->st_mode = 0444;
				break;
		}
	}
}


/*static int fuse_mapper_access(const char *path, int mode)
{
	//TODO: implement me!!!
	return 0;
}*/

static void *fuse_mapper_init(struct fuse_conn_info *conn, struct fuse_config *cfg)
{
	cfg->kernel_cache = 1; //read only file systems can utilize caching, since data never changes

	fuse_context *context = fuse_get_context();
	return context->private_data;
}

static int fuse_mapper_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
{
	Optional<FileInfo> fileInfo = GetState().FileSystem().QueryFileInfo(String(path));
	if(!fileInfo.HasValue())
		return -ENOENT;
	MapPermissions(fileInfo->type, fileInfo->permissions, stbuf);

	//st_dev and st_blksize are ignored (st_ino also)
	stbuf->st_atim = {};
	stbuf->st_ctim = {};

	if(fileInfo->lastModifiedTime.HasValue())
	{
		stbuf->st_mtim.tv_sec = fileInfo->lastModifiedTime->ToUnixTimestamp();
		stbuf->st_mtim.tv_nsec = fileInfo->lastModifiedTime->Time().NanosecondsSinceStartOfSecond();
	}
	else
		stbuf->st_mtim = {};

	switch (fileInfo->type)
	{
		case FileType::Directory:
			stbuf->st_mode |= S_IFDIR;
			stbuf->st_nlink = 2;
			stbuf->st_size = 0;
			stbuf->st_blocks = 0;
			break;
		case FileType::File:
			stbuf->st_mode |= S_IFREG;
			stbuf->st_nlink = 1;
			stbuf->st_size = fileInfo->size;
			stbuf->st_blocks = fileInfo->storedSize / 512;
			break;
		case FileType::Link:
			stbuf->st_mode |= S_IFLNK;
			stbuf->st_nlink = 1;
			stbuf->st_size = fileInfo->size;
			stbuf->st_blocks = fileInfo->storedSize / 512;
			break;
	}

	return 0;
}

/*static int fuse_mapper_getxattr(const char * path, const char * name, char * value, size_t size)
{
	//TODO: implement me
	errno = ENOTSUP;
	return -1;
}*/

static int fuse_mapper_open(const char *path, struct fuse_file_info *fi)
{
	if(!GetState().FileSystem().QueryFileInfo(String(path)).HasValue())
		return -ENOENT;
	if ((fi->flags & O_ACCMODE) != O_RDONLY)
		return -EACCES;

	return 0;
}

static int fuse_mapper_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	Path p = String(path);
	fuse_mapper_state& state = GetState();

	if(!state.FileSystem().QueryFileInfo(p).HasValue())
		return -ENOENT;
		
	InputStream& inputStream = state.GetInputStream(p, offset);
	uint32 nBytesRead = inputStream.ReadBytes(buf, size);
	state.UpdateStream(p, nBytesRead);

	return nBytesRead;
}

static int fuse_mapper_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags)
{
	UniquePointer<DirectoryEnumerator> directoryEnumerator = GetState().FileSystem().EnumerateChildren(String(path));
	if(directoryEnumerator.IsNull())
		return -ENOENT;

	int res = filler(buf, ".", nullptr, 0, static_cast<fuse_fill_dir_flags>(0));
	res = filler(buf, "..", nullptr, 0, static_cast<fuse_fill_dir_flags>(0));

	while(true)
	{
		if(!directoryEnumerator->MoveForward())
			break;

		res = filler(buf, reinterpret_cast<const char *>(directoryEnumerator->GetCurrent().name.ToUTF8().GetRawZeroTerminatedData()), nullptr, 0,
			   static_cast<fuse_fill_dir_flags>(0));
	}

	return 0;
}

static int fuse_mapper_readlink(const char* path, char* buffer, size_t bufferSize)
{
	Path p = String(path);

	Optional<Path> target = GetState().FileSystem().ReadLinkTarget(p);
	if(!target.HasValue())
		return -ENOENT;

	const String& targetString = target->String();
	targetString.ToUTF8();

	uint32 nullByteOffset = Math::Min((uint32)(bufferSize - 1), targetString.GetSize());
	MemCopy(buffer, targetString.GetRawData(), nullByteOffset);
	buffer[nullByteOffset] = 0;

	return 0;
}

void _stdxx_::fuse_MountReadOnly(const Path &mountPoint, const ReadableFileSystem &fileSystem)
{
	struct fuse_operations operations = {
		.getattr = fuse_mapper_getattr,
		.readlink = fuse_mapper_readlink,
		.open = fuse_mapper_open,
		.read = fuse_mapper_read,
		//.getxattr = fuse_mapper_getxattr,
		.readdir = fuse_mapper_readdir,
		.init = fuse_mapper_init,
		//.access = fuse_mapper_access,
	};

	fuseArgsCreator args;

	args.Add(u8""); //program
#ifdef XPC_BUILDTYPE_DEBUG
	args.Add(u8"-d"); //debug output
#endif
	args.Add(u8"-f"); //foreground (i.e. in this thread)
	args.Add(mountPoint.String());

	fuse_mapper_state state(fileSystem);
	fuse_main( args.argc(), args.argv(), &operations, (void*)&state );
}
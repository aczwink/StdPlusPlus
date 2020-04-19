/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/FileSystem/DirectoryIterator.hpp>
#include <Std++/Containers/Map/Map.hpp>
#include <Std++/Multitasking/Mutex.hpp>
#include <Std++/Streams/SeekableInputStream.hpp>
#include <Std++/FileSystem/Link.hpp>
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
	inline fuse_mapper_state(const RWFileSystem& fileSystem) : fileSystem(fileSystem)
	{
	}

	//Methods
	InputStream& GetInputStream(const Path& path, uint64 offset)
	{
		this->openInputStreamsLock.Lock();
		StreamState& streamState = this->openInputStreams[path];

		if(streamState.inputStream.IsNull())
		{
			streamState.inputStream = this->fileSystem.GetFile(path)->OpenForReading(false);
			streamState.offset = 0;
		}
		InputStream* inputStream = streamState.inputStream.operator->();

		if(offset != streamState.offset)
		{
			if(streamState.inputStream.IsInstanceOf<SeekableInputStream>())
			{
				SeekableInputStream& seekableInputStream = *dynamic_cast<SeekableInputStream*>(inputStream);
				seekableInputStream.SetCurrentOffset(offset);
			}
			else if(streamState.offset < offset)
				inputStream->Skip(offset - streamState.offset);
			else
			{
				streamState.inputStream = this->fileSystem.GetFile(path)->OpenForReading(false);
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

	//Inline
	inline AutoPointer<const FileSystemNode> GetNode(const char* path) const
	{
		Path p = String(path);
		NOT_IMPLEMENTED_ERROR; //TODO: REIMPLEMENT NEXT LINEs
		//AutoPointer<const FileSystemNode> node = fileSystem.GetNode(p);
		//return node;
	}

private:
	//Members
	const RWFileSystem& fileSystem;
	Map<Path, StreamState> openInputStreams;
	Mutex openInputStreamsLock;
};

static fuse_mapper_state& GetState()
{
	return *(fuse_mapper_state*) fuse_get_context()->private_data;
}

static void *fuse_mapper_init(struct fuse_conn_info *conn, struct fuse_config *cfg)
{
	cfg->kernel_cache = 1; //read only file systems can utilize caching, since data never changes

	fuse_context *context = fuse_get_context();
	return context->private_data;
}

static int fuse_mapper_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
{
	AutoPointer<const FileSystemNode> node = GetState().GetNode(path);
	if(node.IsNull())
		return -ENOENT;

	//st_dev and st_blksize are ignored (st_ino also)
	stbuf->st_uid = 0;
	stbuf->st_gid = 0;
	stbuf->st_atim = {};
	stbuf->st_ctim = {};

	FileSystemNodeInfo info = node->QueryInfo();
	if(info.lastModifiedTime.HasValue())
	{
		stbuf->st_mtim.tv_sec = info.lastModifiedTime->ToUnixTimestamp();
		stbuf->st_mtim.tv_nsec = info.lastModifiedTime->Time().NanosecondsSinceStartOfSecond();
	}
	else
		stbuf->st_mtim = {};

	switch (node->GetType())
	{
		case FileSystemNodeType::Directory:
			stbuf->st_mode = S_IFDIR | 0555;
			stbuf->st_nlink = 2;
			stbuf->st_size = 0;
			stbuf->st_blocks = 0;
			break;
		case FileSystemNodeType::File:
			stbuf->st_mode = S_IFREG | 0444;
			stbuf->st_nlink = 1;
			stbuf->st_size = node.Cast<const File>()->GetSize();
			stbuf->st_blocks = info.storedSize / 512;
			break;
		case FileSystemNodeType::Link:
			stbuf->st_mode = S_IFLNK | 0555;
			stbuf->st_nlink = 1;
			stbuf->st_size = info.storedSize;
			stbuf->st_blocks = info.storedSize / 512;
			break;
	}

	return 0;
}

static int fuse_mapper_open(const char *path, struct fuse_file_info *fi)
{
	AutoPointer<const FileSystemNode> node = GetState().GetNode(path);
	if(node.IsNull())
		return -ENOENT;
	if ((fi->flags & O_ACCMODE) != O_RDONLY)
		return -EACCES;

	return 0;
}

static int fuse_mapper_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	fuse_mapper_state &state = GetState();
	Path p = String::CopyRawString(path);
	AutoPointer<const FileSystemNode> node = state.GetNode(path);
	if(node.IsNull())
		return -ENOENT;
	if(node->GetType() == FileSystemNodeType::Directory)
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		
	InputStream& inputStream = state.GetInputStream(p, offset);
	uint32 nBytesRead = inputStream.ReadBytes(buf, size);
	state.UpdateStream(p, nBytesRead);

	return nBytesRead;
}

static int fuse_mapper_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags)
{
	AutoPointer<const FileSystemNode> node = GetState().GetNode(path);
	if(node.IsNull())
		return -ENOENT;
	if(node->GetType() != FileSystemNodeType::Directory)
		return -ENOENT;

	filler(buf, u8".", NULL, 0, static_cast<fuse_fill_dir_flags>(0));
	filler(buf, u8"..", NULL, 0, static_cast<fuse_fill_dir_flags>(0));

	AutoPointer<const Directory> dir = node.Cast<const Directory>();
	for(const String& childName : *dir)
	{
		filler(buf, reinterpret_cast<const char *>(childName.ToUTF8().GetRawZeroTerminatedData()), nullptr, 0,
			   static_cast<fuse_fill_dir_flags>(0));
	}

	return 0;
}

static int fuse_mapper_readlink(const char* path, char* buffer, size_t bufferSize)
{
	AutoPointer<const FileSystemNode> node = GetState().GetNode(path);
	if(node.IsNull())
		return -ENOENT;
	if(node->GetType() != FileSystemNodeType::Link)
		return -ENOENT;

	AutoPointer<const Link> link = node.Cast<const Link>();
	Path target = link->ReadTarget();
	const String& targetString = target.GetString();
	targetString.ToUTF8();

	uint32 nullByteOffset = Math::Min((uint32)(bufferSize - 1), targetString.GetSize());
	MemCopy(buffer, targetString.GetRawData(), nullByteOffset);
	buffer[nullByteOffset] = 0;

	return 0;
}

void _stdxx_::fuse_MountReadOnly(const Path &mountPoint, const RWFileSystem &fileSystem)
{
	struct fuse_operations operations = {
		.getattr = fuse_mapper_getattr,
		.readlink = fuse_mapper_readlink,
		.open = fuse_mapper_open,
		.read = fuse_mapper_read,
		.readdir = fuse_mapper_readdir,
		.init = fuse_mapper_init,
	};

	fuseArgsCreator args;

	args.Add(u8""); //program
#ifdef XPC_BUILDTYPE_DEBUG
	args.Add(u8"-d"); //debug output
#endif
	args.Add(u8"-f"); //foreground (i.e. in this thread)
	args.Add(mountPoint.GetString());

	fuse_mapper_state state(fileSystem);
	fuse_main( args.argc(), args.argv(), &operations, (void*)&state );
}
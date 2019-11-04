/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
//Interface header
#include "PosixStat.hpp"
//Global
#include <sys/stat.h>
#include <errno.h>
//Local
#include "POSIXFile.hpp"
#include "POSIXDirectory.hpp"
#include "PosixLink.hpp"
//Namespaces
using namespace StdXX;

//Namespace functions
template<typename ConstOrNotFileSystemNode>
AutoPointer<ConstOrNotFileSystemNode> _stdxx_::StatFindNode(const Path& path)
{
	struct stat sb{};

	int result = lstat(reinterpret_cast<const char *>(path.GetString().ToUTF8().GetRawZeroTerminatedData()), &sb);
	if(result == -1)
	{
		switch(errno)
		{
			case EACCES:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EIO:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ELOOP:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENAMETOOLONG:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENOENT:
				return nullptr;
			case ENOTDIR:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EOVERFLOW:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			default:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	}
	else if(result != 0)
	{
		//forbidden as of posix
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	if(S_ISDIR(sb.st_mode))
		return new POSIXDirectory(path);
	if(S_ISLNK(sb.st_mode))
		return new PosixLink(path);
	if(S_ISREG(sb.st_mode))
		return new POSIXFile(path);

	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

//Explicit instantiation
template AutoPointer<FileSystemNode> _stdxx_::StatFindNode(const Path& path);
template AutoPointer<const FileSystemNode> _stdxx_::StatFindNode(const Path& path);

FileSystemNodeInfo _stdxx_::StatQueryFileInfo(const Path &path, uint64 &fileSize)
{
	struct stat sb{};
	int ret = lstat(reinterpret_cast<const char *>(path.GetString().ToUTF8().GetRawZeroTerminatedData()), &sb);
	ASSERT(ret == 0, u8"REPORT THIS PLEASE!");

	FileSystemNodeInfo info{};

	//set file size
	fileSize = static_cast<uint64>(sb.st_size);

	//set last modified time
#ifdef XPC_OS_DARWIN
    FileSystemNodeInfo::FileSystemNodeTime lastModTime{DateTime::FromUnixTimeStamp(sb.st_mtimespec.tv_sec)};
    lastModTime.ns = static_cast<uint64>(sb.st_mtimespec.tv_nsec);
#else
    FileSystemNodeInfo::FileSystemNodeTime lastModTime{DateTime::FromUnixTimeStamp(sb.st_mtim.tv_sec)};
	lastModTime.ns = static_cast<uint64>(sb.st_mtim.tv_nsec);
#endif
	info.lastModifiedTime = lastModTime;

	//set stored size
#ifdef XPC_OS_LINUX
	//sb.st_size can be larger in case the file has "holes"...
	//on linux st_blocks is in 512 units
	info.storedSize = Math::Max(static_cast<uint64>(sb.st_blocks * 512), fileSize);
#else
	info.storedSize = fileSize;
#endif

	return info;
}
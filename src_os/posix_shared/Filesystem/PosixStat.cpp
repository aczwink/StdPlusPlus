/*
 * Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/FileSystem/POSIXPermissions.hpp>
#include "POSIXFile.hpp"
#include "POSIXDirectory.hpp"
#include "PosixLink.hpp"
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

//Local functions
template<typename SecondsType, typename NanoSecondsType>
static DateTime WithNanoSecondsToTime(SecondsType seconds, NanoSecondsType nanoseconds)
{
	return DateTime::FromUnixTimeStamp(seconds).AddNanoseconds(nanoseconds);
}

//Namespace functions
template<typename ConstOrNotFileSystemNode>
AutoPointer<ConstOrNotFileSystemNode> _stdxx_::StatFindNode(const Path& path)
{
	struct stat sb{};

	int result = lstat(reinterpret_cast<const char *>(path.String().ToUTF8().GetRawZeroTerminatedData()), &sb);
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
template AutoPointer<Node> _stdxx_::StatFindNode(const Path& path);
template AutoPointer<const Node> _stdxx_::StatFindNode(const Path& path);

NodeInfo _stdxx_::StatQueryFileInfo(const Path &path)
{
	struct stat sb{};
	int ret = lstat(reinterpret_cast<const char *>(path.String().ToUTF8().GetRawZeroTerminatedData()), &sb);
	ASSERT(ret == 0, u8"REPORT THIS PLEASE!");

	NodeInfo info{};

	//set file size
	info.size = static_cast<uint64>(sb.st_size);

	//set last modified time
#ifdef XPC_OS_DARWIN
	info.lastModifiedTime = WithNanoSecondsToTime(sb.st_mtimespec.tv_sec, sb.st_mtimespec.tv_nsec);
#else
	info.lastModifiedTime = WithNanoSecondsToTime(sb.st_mtim.tv_sec, sb.st_mtim.tv_nsec);
#endif

	//set stored size
#if defined(XPC_OS_LINUX) || defined(XPC_OS_DARWIN)
	//sb.st_size can be larger in case the file has "holes"...
	//on linux and darwin st_blocks is in 512 units
	info.storedSize = sb.st_blocks * 512_u64;
#else
	info.storedSize = info.size;
#endif

	//permissions
	info.permissions = new FileSystem::POSIXPermissions(sb.st_uid, sb.st_gid, sb.st_mode);

	return info;
}
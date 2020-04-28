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
//Class header
#include "POSIXDirectory.hpp"
//Global
#include <cerrno>
#include <sys/stat.h>
//Local
#include <Std++/FileSystem/DirectoryIterator.hpp>
#include <Std++/Errorhandling/Exceptions/PermissionDeniedException.hpp>
#include <Std++/FileSystem/POSIXPermissions.hpp>
#include "POSIXDirectoryIteratorState.hpp"
#include "POSIXFile.hpp"
#include "PosixStat.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::FileSystem;

//Public methods
UniquePointer<OutputStream> POSIXDirectory::CreateFile(const String &name)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

void POSIXDirectory::CreateSubDirectory(const String &name, const NodePermissions* permissions)
{
	Path p = this->path / name;

	uint32 permissionFlags = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
	if(permissions)
    {
	    const POSIXPermissions* posixPermissions = dynamic_cast<const POSIXPermissions *>(permissions);
	    ASSERT(posixPermissions, u8"Illegal permissions type");
	    permissionFlags = posixPermissions->EncodeMode();
    }
	int ret = mkdir(reinterpret_cast<const char *>(p.String().ToUTF8().GetRawZeroTerminatedData()), permissionFlags);
	if(ret)
	{
		switch(errno)
		{
			case EACCES:
				throw ErrorHandling::PermissionDeniedException(p);
			case EEXIST:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ELOOP:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EMLINK:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENAMETOOLONG:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENOENT:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENOSPC:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENOTDIR:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EROFS:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	}
}

bool POSIXDirectory::Exists(const Path &path) const
{
	Path p = this->path / path;
	struct stat sb{};
	return stat(reinterpret_cast<const char *>(p.String().ToUTF8().GetRawZeroTerminatedData()), &sb) == 0;
}

AutoPointer<Node> POSIXDirectory::GetChild(const String &name)
{
	return StatFindNode<Node>(this->path / name);
}

AutoPointer<const Node> POSIXDirectory::GetChild(const String &name) const
{
	return StatFindNode(this->path / name);
}

bool POSIXDirectory::IsEmpty() const
{
	for(const String& child : *this)
		return false;
	return true;
}

//For range-based loop
StdXX::FileSystem::DirectoryIterator _stdxx_::POSIXDirectory::begin() const
{
	return new POSIXDirectoryIteratorState(this->path);
}

StdXX::FileSystem::DirectoryIterator _stdxx_::POSIXDirectory::end() const
{
	return nullptr;
}



//OLD:
/*bool POSIXDirectory::ContainsFile(const String &name) const
{
	Path p = this->path / name;
	struct stat sb{};
	return stat(reinterpret_cast<const char *>(p.GetString().ToUTF8().GetRawZeroTerminatedData()), &sb) == 0 && S_ISDIR(sb.st_mode) == 0;
}

bool POSIXDirectory::ContainsSubDirectory(const String &name) const
{
	Path p = this->path / name;
	struct stat sb{};
	return stat(reinterpret_cast<const char *>(p.String().ToUTF8().GetRawZeroTerminatedData()), &sb) == 0 && S_ISDIR(sb.st_mode) != 0;
}
 */
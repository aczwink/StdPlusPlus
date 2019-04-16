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
//Class header
#include "POSIXDirectory.hpp"
//Global
#include <sys/stat.h>
//Local
#include <Std++/Filesystem/DirectoryIterator.hpp>
#include "POSIXDirectoryIteratorState.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Public methods
bool POSIXDirectory::ContainsFile(const String &name) const
{
	Path p = this->path / name;
	struct stat sb{};
	return stat(reinterpret_cast<const char *>(p.GetString().ToUTF8().GetRawZeroTerminatedData()), &sb) == 0 && S_ISDIR(sb.st_mode) == 0;
}

bool POSIXDirectory::ContainsSubDirectory(const String &name) const
{
	Path p = this->path / name;
	struct stat sb{};
	return stat(reinterpret_cast<const char *>(p.GetString().ToUTF8().GetRawZeroTerminatedData()), &sb) == 0 && S_ISDIR(sb.st_mode) != 0;
}

UniquePointer<OutputStream> POSIXDirectory::CreateFile(const String &name)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

void POSIXDirectory::CreateSubDirectory(const String &name)
{
	Path p = this->path / name;
	bool success = mkdir(reinterpret_cast<const char *>(p.GetString().ToUTF8().GetRawZeroTerminatedData()), 0700) == 0;
}

bool POSIXDirectory::Exists(const Path &path) const
{
	Path p = this->path / path;
	struct stat sb{};
	return stat(reinterpret_cast<const char *>(p.GetString().ToUTF8().GetRawZeroTerminatedData()), &sb) == 0;
}

AutoPointer<const File> POSIXDirectory::GetFile(const String &name) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::AutoPointer<const StdXX::File>();
}

FileSystem *POSIXDirectory::GetFileSystem()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

const FileSystem *POSIXDirectory::GetFileSystem() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

AutoPointer<const Directory> POSIXDirectory::GetParent() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

Path POSIXDirectory::GetPath() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return Path();
}

uint64 POSIXDirectory::GetSize() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

uint64 POSIXDirectory::GetStoredSize() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

AutoPointer<Directory> POSIXDirectory::GetSubDirectory(const String &name)
{
	return new POSIXDirectory(this->path / name);
}

AutoPointer<const Directory> POSIXDirectory::GetSubDirectory(const String &name) const
{
	return new POSIXDirectory(this->path / name);
}

//For range-based loop
StdXX::DirectoryIterator _stdxx_::POSIXDirectory::begin() const
{
	return new POSIXDirectoryIteratorState(this->path);
}

StdXX::DirectoryIterator _stdxx_::POSIXDirectory::end() const
{
	return nullptr;
}
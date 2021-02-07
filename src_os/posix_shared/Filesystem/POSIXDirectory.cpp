/*
 * Copyright (c) 2019-2021 Amir Czwink (amir130@hotmail.de)
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
/*//Class header
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
bool POSIXDirectory::Exists(const Path &path) const
{
	Path p = this->path / path;
	struct stat sb{};
	return stat(reinterpret_cast<const char *>(p.String().ToUTF8().GetRawZeroTerminatedData()), &sb) == 0;
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
 */
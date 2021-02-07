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
//Class header
#include "POSIXDirectoryEnumerator.hpp"
//Local
#include <Std++/Errorhandling/Exceptions/FileNotFoundException.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::FileSystem;

//Constructor
POSIXDirectoryEnumerator::POSIXDirectoryEnumerator(const Path& path)
{
	this->dir = opendir(reinterpret_cast<const char *>(path.String().ToUTF8().GetRawZeroTerminatedData()));
	if(!this->dir)
		throw ErrorHandling::FileNotFoundException(path);
}

//Destructor
POSIXDirectoryEnumerator::~POSIXDirectoryEnumerator()
{
	closedir(this->dir);
}

//Public methods
bool POSIXDirectoryEnumerator::Next(DirectoryEntry& directoryEntry)
{
	dirent* currentEntry = readdir(this->dir);
	if(currentEntry)
	{
		switch(currentEntry->d_type)
		{
			case DT_DIR:
				directoryEntry.type = FileType::Directory;
				break;
			case DT_LNK:
				directoryEntry.type = FileType::Link;
				break;
			case DT_REG:
				directoryEntry.type = FileType::File;
				break;
			default:
				NOT_IMPLEMENTED_ERROR;
		}
		directoryEntry.name = String::CopyRawString(currentEntry->d_name);

		return true;
	}
	return false;
}
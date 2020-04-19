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
#include "POSIXDirectoryIteratorState.hpp"
//Global
#include <sys/stat.h>
//Local
#include <Std++/Errorhandling/Exceptions/FileNotFoundException.hpp>
#include "POSIXDirectory.hpp"
#include "POSIXFile.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Constructor
POSIXDirectoryIteratorState::POSIXDirectoryIteratorState(const FileSystem::Path& path) : path(path)
{
	this->dir = opendir(reinterpret_cast<const char *>(path.GetString().ToUTF8().GetRawZeroTerminatedData()));
	if(!this->dir)
		throw ErrorHandling::FileNotFoundException(path);
	this->Next();
}

//Destructor
POSIXDirectoryIteratorState::~POSIXDirectoryIteratorState()
{
	closedir(this->dir);
}

//Public methods
bool POSIXDirectoryIteratorState::Equals(DirectoryIteratorState *other) const
{
	//if other is null, we are equal if we are also null
	if(other == nullptr)
		return this->currentEntry == nullptr;

	if(IS_INSTANCE_OF(other, POSIXDirectoryIteratorState))
	{
		POSIXDirectoryIteratorState* otherTyped = dynamic_cast<POSIXDirectoryIteratorState *>(other);
		return otherTyped->currentEntry == this->currentEntry;
	}

	return false;
}

String POSIXDirectoryIteratorState::GetCurrent()
{
	return String::CopyRawString(this->currentEntry->d_name);
}

void POSIXDirectoryIteratorState::Next()
{
	this->currentEntry = readdir(this->dir);
	while(this->currentEntry) //unfortunately, the order of the returned files is not known. Therefore we don't know when "." and ".." will arrive...
	{
		bool isDot = (this->currentEntry->d_name[0] == u8'.') && (this->currentEntry->d_name[1] == 0);
		bool isDotDot = (this->currentEntry->d_name[0] == u8'.') && (this->currentEntry->d_name[1] == u8'.') && (this->currentEntry->d_name[2] == 0);
		if(isDot || isDotDot)
		{
			this->currentEntry = readdir(this->dir);
			continue;
		}
		break;
	}
}

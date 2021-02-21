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
#include <Std++/Streams/FileUpdateStream.hpp>
//Global
#include <unistd.h>
//Namespaces
using namespace StdXX;

//Public methods
uint64 FileUpdateStream::QueryCurrentOffset() const
{
	return (uint64)lseek64(this->fileHandle, 0, SEEK_CUR);
}

uint64 FileUpdateStream::QuerySize() const
{
	uint64 offset = this->QueryCurrentOffset();
	uint64 size = (uint64) lseek64(this->fileHandle, 0, SEEK_END);
	lseek64(this->fileHandle, offset, SEEK_SET);

	return size;
}

void FileUpdateStream::SeekTo(uint64 offset)
{
	lseek64(this->fileHandle, offset, SEEK_SET);
}
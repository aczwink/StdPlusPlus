/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
//Class Header
#include <Std++/Streams/FileInputStream.hpp>
//Global
#include <unistd.h>
//Namespaces
using namespace StdXX;

static_assert(sizeof(off_t) >= 8); //make sure we can read large files

//Public methods
uint64 FileInputStream::GetCurrentOffset() const
{
	return (uint64)lseek(this->fileHandle, 0, SEEK_CUR);
}

uint64 FileInputStream::GetSize() const
{
	uint64 offset, size;

	offset = this->GetCurrentOffset();
	size = (uint64)lseek(this->fileHandle, 0, SEEK_END);
	lseek(this->fileHandle, offset, SEEK_SET);

	return size;
}

void FileInputStream::SetCurrentOffset(uint64 offset)
{
	lseek(this->fileHandle, offset, SEEK_SET);
}

uint32 FileInputStream::Skip(uint32 nBytes)
{
	uint64 currentOffset;

	currentOffset = this->GetCurrentOffset();
	return (uint32)(lseek(this->fileHandle, nBytes, SEEK_CUR) - currentOffset);
}
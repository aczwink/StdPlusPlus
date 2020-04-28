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
//Class Header
#include <Std++/Streams/FileUpdateStream.hpp>
//Global
#include <unistd.h>
//Namespaces
using namespace StdXX;

static_assert(sizeof(off_t) >= 8); //make sure we can read large files

uint64 FileUpdateStream::GetCurrentOffset() const
{
    return lseek(this->fileHandle, 0, SEEK_CUR);
}

uint64 FileUpdateStream::QuerySize() const
{
    uint64 offset = this->GetCurrentOffset();
    off_t size = lseek(this->fileHandle, 0, SEEK_END);
    lseek(this->fileHandle, offset, SEEK_SET);

    return size;
}

void FileUpdateStream::SeekTo(uint64 offset)
{
    lseek(this->fileHandle, offset, SEEK_SET);
}
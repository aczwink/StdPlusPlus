/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/FileOutputStream.hpp>
//Global
#include <fcntl.h>
#include <unistd.h>
//Local
#include <Std++/Containers/Strings/UTF-8/UTF8String.hpp>
//Namespaces
using namespace StdPlusPlus;

//Constructor
FileOutputStream::FileOutputStream(const Path &refPath)
{
    this->fileHandle = open(reinterpret_cast<const char *>(refPath.GetString().ToUTF8().GetRawZeroTerminatedData()), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
}

//Destructor
FileOutputStream::~FileOutputStream()
{
    close(this->fileHandle);
}

//Public methods
uint64 FileOutputStream::GetCurrentOffset() const
{
    return lseek64(this->fileHandle, 0, SEEK_CUR);
}

void FileOutputStream::SetCurrentOffset(uint64 offset)
{
    lseek64(this->fileHandle, offset, SEEK_SET);
}

void FileOutputStream::WriteByte(byte b)
{
    write(this->fileHandle, &b, 1);
}

uint32 FileOutputStream::WriteBytes(const void *pSource, uint32 size)
{
    return write(this->fileHandle, pSource, size);
}
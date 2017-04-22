/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class Header
#include <ACStdLib/Streams/CFileInputStream.h>
//Global
#include <fcntl.h>
#include <unistd.h>
//Local
#include <ACStdLib/Containers/Strings/UTF-8/CUTF8String.h>
#include <ACStdLib/ErrorHandling/CFileNotFoundException.hpp>
//Namespaces
using namespace ACStdLib;

//Constructor
CFileInputStream::CFileInputStream(const CPath &refPath)
{
    CUTF8String fileNameUTF8(refPath.GetString().GetUTF16());

    this->hitEnd = false;
    this->fileHandle = open((const char *)fileNameUTF8.GetC_Str(), O_RDONLY);
}

//Destructor
CFileInputStream::~CFileInputStream()
{
    close(this->fileHandle);
}

//Public methods
uint64 CFileInputStream::GetCurrentOffset() const
{
    return lseek64(this->fileHandle, 0, SEEK_CUR);
}

uint64 CFileInputStream::GetRemainingBytes() const
{
    return this->GetSize() - this->GetCurrentOffset();
}

uint64 CFileInputStream::GetSize() const
{
    uint64 offset, size;

    offset = this->GetCurrentOffset();
    size = lseek64(this->fileHandle, 0, SEEK_END);
    lseek64(this->fileHandle, offset, SEEK_CUR);

    return size;
}

byte CFileInputStream::ReadByte()
{
    byte b;

    this->hitEnd = read(this->fileHandle, &b, 1) != 1;

    return b;
}

uint32 CFileInputStream::ReadBytes(void *pDestination, uint32 count)
{
    uint32 nReadBytes;

    nReadBytes = read(this->fileHandle, pDestination, count);
    this->hitEnd = nReadBytes < count;

    return nReadBytes;
}

void CFileInputStream::SetCurrentOffset(uint64 offset)
{
    lseek64(this->fileHandle, offset, SEEK_SET);
}

uint32 CFileInputStream::Skip(uint32 nBytes)
{
    uint64 currentOffset;

    currentOffset = this->GetCurrentOffset();
    return (uint32)(lseek64(this->fileHandle, nBytes, SEEK_CUR) - currentOffset);
}
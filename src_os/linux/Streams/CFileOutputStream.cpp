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
#include <ACStdLib/Streams/CFileOutputStream.h>
//Global
#include <fcntl.h>
#include <unistd.h>
//Local
#include <ACStdLib/Containers/Strings/UTF-8/CUTF8String.h>
//Namespaces
using namespace ACStdLib;

//Constructor
CFileOutputStream::CFileOutputStream(const CPath &refPath)
{
    CUTF8String fileNameUTF8(refPath.GetString().GetUTF16());

    this->fileHandle = open((const char *)fileNameUTF8.GetC_Str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
}

//Destructor
CFileOutputStream::~CFileOutputStream()
{
    close(this->fileHandle);
}

//Public methods
uint64 CFileOutputStream::GetCurrentOffset() const
{
    return lseek64(this->fileHandle, 0, SEEK_CUR);
}

void CFileOutputStream::SetCurrentOffset(uint64 offset)
{
    lseek64(this->fileHandle, offset, SEEK_SET);
}

void CFileOutputStream::WriteByte(byte b)
{
    write(this->fileHandle, &b, 1);
}

uint32 CFileOutputStream::WriteBytes(const void *pSource, uint32 size)
{
    return write(this->fileHandle, pSource, size);
}
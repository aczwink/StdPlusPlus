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
#include <Std++/Streams/FileUpdateStream.hpp>
//Global
#include <fcntl.h>
#include <unistd.h>
//Local
#include "read.hpp"
//Namespaces
using namespace StdXX;

//Constructor
FileUpdateStream::FileUpdateStream(const FileSystem::Path &path)
{
	int flags = O_RDWR;
	this->fileHandle = open(reinterpret_cast<const char *>(path.String().ToUTF8().GetRawZeroTerminatedData()), flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	ASSERT(this->fileHandle != -1, u8"REPORT THIS PLEASE!");
}

//Destructor
FileUpdateStream::~FileUpdateStream()
{
	close(this->fileHandle);
}

void FileUpdateStream::Flush()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

uint64 FileUpdateStream::QueryRemainingBytes() const
{
	return this->QuerySize() - this->GetCurrentOffset();
}

uint32 FileUpdateStream::ReadBytes(void *destination, uint32 count)
{
	return POSIXReadBytes(this->fileHandle, destination, count);
}

uint32 FileUpdateStream::Skip(uint32 nBytes)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

uint32 FileUpdateStream::WriteBytes(const void *source, uint32 size)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

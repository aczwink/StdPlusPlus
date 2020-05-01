/*
 * Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
#include <fcntl.h>
#include <unistd.h>
//Local
#include <Std++/Errorhandling/Exceptions/FileNotFoundException.hpp>
#include "read.hpp"
//Namespaces
using namespace StdXX;

//Constructor
FileInputStream::FileInputStream(const FileSystem::Path &path)
{
	this->fileHandle = open(reinterpret_cast<const char *>(path.String().ToUTF8().GetRawZeroTerminatedData()), O_RDONLY);

	if(this->fileHandle == -1)
	{
		switch(errno)
		{
			case ENOENT:
				throw ErrorHandling::FileNotFoundException(path);
			default:
			{
				int errno_bkp = errno;
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			}
		}
	}
}

//Destructor
FileInputStream::~FileInputStream()
{
	close(this->fileHandle);
}

//Public methods
uint64 FileInputStream::QueryRemainingBytes() const
{
	return this->QuerySize() - this->GetCurrentOffset();
}

uint32 FileInputStream::ReadBytes(void* destination, uint32 count)
{
	return POSIXReadBytes(this->fileHandle, destination, count);
}
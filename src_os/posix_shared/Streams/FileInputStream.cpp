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
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
//Local
#include <Std++/Errorhandling/FileNotFoundException.hpp>
//Namespaces
using namespace StdPlusPlus;

//Constructor
FileInputStream::FileInputStream(const Path &path)
{
	this->hitEnd = false;
	this->fileHandle = open(reinterpret_cast<const char *>(path.GetString().ToUTF8().GetRawZeroTerminatedData()), O_RDONLY);

	if(this->fileHandle == -1)
	{
		switch(errno)
		{
			case ENOENT:
				throw ErrorHandling::FileNotFoundException(path);
		}
	}
}

//Destructor
FileInputStream::~FileInputStream()
{
	close(this->fileHandle);
}

//Public methods
uint64 FileInputStream::GetRemainingBytes() const
{
	return this->GetSize() - this->GetCurrentOffset();
}

uint32 FileInputStream::ReadBytes(void *pDestination, uint32 count)
{
	uint32 nReadBytes;

	nReadBytes = read(this->fileHandle, pDestination, count);
	this->hitEnd = nReadBytes < count;

	return nReadBytes;
}
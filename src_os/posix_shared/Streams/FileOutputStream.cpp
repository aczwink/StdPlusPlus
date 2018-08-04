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
#include <Std++/Streams/FileOutputStream.hpp>
//Global
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
//Local
#include <Std++/Errorhandling/FileAlreadyExistsException.hpp>
//Namespaces
using namespace StdXX;

//Constructor
FileOutputStream::FileOutputStream(const Path &path, bool overwrite) : filePath(path)
{
	int flags = O_WRONLY | O_CREAT | O_TRUNC;
	if(!overwrite)
		flags |= O_EXCL;
	this->fileHandle = open(reinterpret_cast<const char *>(path.GetString().ToUTF8().GetRawZeroTerminatedData()), flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if(this->fileHandle == -1)
	{
		switch(errno)
		{
			case EEXIST:
				throw ErrorHandling::FileAlreadyExistsException(path);
			default:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	}
}

//Destructor
FileOutputStream::~FileOutputStream()
{
	close(this->fileHandle);
}

//Public methods
uint32 FileOutputStream::WriteBytes(const void *pSource, uint32 size)
{
	return write(this->fileHandle, pSource, size);
}
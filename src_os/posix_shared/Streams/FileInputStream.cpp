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
			case EACCES:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EEXIST:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EINTR:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EINVAL:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EIO:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EISDIR:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ELOOP:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EMFILE:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENAMETOOLONG:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENFILE:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENOTDIR:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENOSR:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENOSPC:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENXIO:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EOVERFLOW:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EROFS:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EAGAIN:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENOMEM:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EOPNOTSUPP:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ETXTBSY:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
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
	return this->QuerySize() - this->QueryCurrentOffset();
}

uint32 FileInputStream::ReadBytes(void* destination, uint32 count)
{
	return POSIXReadBytes(this->fileHandle, destination, count);
}

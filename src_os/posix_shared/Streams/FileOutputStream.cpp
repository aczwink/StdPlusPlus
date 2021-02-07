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
#include <Std++/Streams/FileOutputStream.hpp>
//Global
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
//Local
#include <Std++/Errorhandling/Exceptions/FileAlreadyExistsException.hpp>
#include <Std++/Errorhandling/Exceptions/FileNotFoundException.hpp>
#include <Std++/FileSystem/POSIXPermissions.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

//Constructor
FileOutputStream::FileOutputStream(const FileSystem::Path &path, bool overwrite, const FileSystem::Permissions* permissions) : filePath(path)
{
	int flags = O_WRONLY | O_CREAT | O_TRUNC;
	if(!overwrite)
		flags |= O_EXCL;

    uint32 permissionFlags = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    if(permissions)
    {
        const POSIXPermissions* posixPermissions = dynamic_cast<const POSIXPermissions *>(permissions);
        ASSERT(posixPermissions, u8"Illegal permissions type");
        permissionFlags = posixPermissions->EncodeMode();
    }

	this->fileHandle = open(reinterpret_cast<const char *>(path.String().ToUTF8().GetRawZeroTerminatedData()), flags, permissionFlags);
	if(this->fileHandle == -1)
	{
		switch(errno)
		{
			case EACCES:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EEXIST:
				throw ErrorHandling::FileAlreadyExistsException(path);
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
			case ENOENT:
			{
				if(path.GetName().IsEmpty())
					NOT_IMPLEMENTED_ERROR; //TODO: inform user that he is stupid
				throw ErrorHandling::FileNotFoundException(path.GetParent());
			}
			case ENOSR:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENOSPC:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENOTDIR:
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
FileOutputStream::~FileOutputStream()
{
	close(this->fileHandle);
}

//Public methods
void FileOutputStream::Flush()
{
	fsync(this->fileHandle);
}

uint32 FileOutputStream::WriteBytes(const void *pSource, uint32 size)
{
	ssize_t result = write(this->fileHandle, pSource, size);

	if(result == -1)
	{
		if(errno == EWOULDBLOCK)
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		switch(errno)
		{
			case EAGAIN:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EBADF:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EFBIG:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EINTR:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EIO:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENOSPC:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ERANGE:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ECONNRESET:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EPIPE:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EINVAL:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENOBUFS:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENXIO:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EACCES:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENETDOWN:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENETUNREACH:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			default:
			{
				int errno_bkp = errno;
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			}
		}
	}

	return result;
}
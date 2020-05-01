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
//Global
#include <errno.h>

inline uint32 POSIXReadBytes(int fileHandle, void* destination, uint32 count)
{
	ssize_t result = read(fileHandle, destination, count);
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
			case EBADMSG:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EINTR:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EINVAL:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EIO:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EISDIR:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case EOVERFLOW:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ESPIPE:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ECONNRESET:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENOTCONN:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ETIMEDOUT:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENOBUFS:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENOMEM:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			case ENXIO:
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
/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include "TCPSocketInputStream.hpp"
//Global
#include <errno.h>
#include <sys/ioctl.h>
//Local
#include <Std++/Mathematics.hpp>
#include "Shared.hpp"
//Namespaces
using namespace _stdxx_;

//Destructor
TCPSocketInputStream::~TCPSocketInputStream()
{
	shutdown(this->socketSystemHandle.i32, SHUT_RD);
}

//Public methods
uint32 TCPSocketInputStream::GetBytesAvailable() const
{
	int bytesAvailable;
	ioctl(this->socketSystemHandle.i32, FIONREAD, &bytesAvailable);
	return static_cast<uint32>(bytesAvailable);
}

bool TCPSocketInputStream::IsAtEnd() const
{
	return false; //we never know whether more data follows or not
}

uint32 TCPSocketInputStream::ReadBytes(void *destination, uint32 count)
{
	uint32 bytesReceivedTotal = 0;
	while(count)
	{
		ssize_t bytesReceived = recv(this->socketSystemHandle.i32, (char*)((byte*)destination + bytesReceivedTotal), count, 0); //winsock wants char* -.-
		if(bytesReceived == -1)
		{
			switch(errno)
			{
				case EWOULDBLOCK:
					NOT_IMPLEMENTED_ERROR; //TODO: handle error
				case EBADF:
					NOT_IMPLEMENTED_ERROR; //TODO: handle error
				case ECONNREFUSED:
					NOT_IMPLEMENTED_ERROR; //TODO: handle error
				case EFAULT:
					NOT_IMPLEMENTED_ERROR; //TODO: handle error
				case EINTR:
					NOT_IMPLEMENTED_ERROR; //TODO: handle error
				case EINVAL:
					NOT_IMPLEMENTED_ERROR; //TODO: handle error
				case ENOMEM:
					NOT_IMPLEMENTED_ERROR; //TODO: handle error
				case ENOTCONN:
					NOT_IMPLEMENTED_ERROR; //TODO: handle error
				case ENOTSOCK:
					NOT_IMPLEMENTED_ERROR; //TODO: handle error
				default:
					NOT_IMPLEMENTED_ERROR; //TODO: handle error
			}
		}

		count -= bytesReceived;
		bytesReceivedTotal += bytesReceived;
	}
	return bytesReceivedTotal;
}

uint32 TCPSocketInputStream::Skip(uint32 nBytes)
{
	uint32 bytesSkipped = 0;
	while(nBytes)
	{
		byte buffer[4096];

		uint32 recvSize = Math::Min(uint32(sizeof(buffer)), nBytes);
		ssize_t bytesReceived = recv(this->socketSystemHandle.i32, (char*)buffer, recvSize, 0); //winsock wants char* -.-
		if(bytesReceived == -1)
		{
			NOT_IMPLEMENTED_ERROR; //TODO: handle error
		}

		nBytes -= bytesReceived;
		bytesSkipped += bytesReceived;
	}
	return bytesSkipped;
}
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
#include "TCPSocketOutputStream.hpp"
//Local
#include "Shared.hpp"
//Namespaces
using namespace _stdxx_;

//Destructor
TCPSocketOutputStream::~TCPSocketOutputStream()
{
#ifdef XPC_OS_WINDOWS
	shutdown(this->socketSystemHandle.i32, SD_SEND);
#else
	shutdown(this->socketSystemHandle.i32, SHUT_WR);
#endif
}

//Public methods
void TCPSocketOutputStream::Flush()
{
	//tcp decides itself when to send data over network
}

uint32 TCPSocketOutputStream::WriteBytes(const void *source, uint32 size)
{
	uint32 bytesSent = 0;
	while(bytesSent < size)
	{
#ifdef XPC_OS_WINDOWS
		ssize_t result = send(this->socketSystemHandle.u64, (char*)(static_cast<const uint8 *>(source) + bytesSent), size - bytesSent, 0);
#else
		ssize_t result = send(this->socketSystemHandle.i32, static_cast<const uint8 *>(source) + bytesSent, size - bytesSent, 0);
#endif
		bytesSent += result;
	}

	return bytesSent;
}

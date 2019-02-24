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
#include <Std++/Network/TCPSocket.hpp>
//Global
#include <errno.h>
#include <fcntl.h>
#ifndef XPC_OS_WINDOWS
#include <errno.h>
#include <poll.h>
#include <unistd.h>
#include <Std++/Signed.hpp>

#endif
//Local
#include "Shared.hpp"
#include "TCPSocketInputStream.hpp"
#include "TCPSocketOutputStream.hpp"
//Namespaces
using namespace StdXX;

//Constructors
TCPSocket::TCPSocket(const Variant& systemHandle) : systemHandle(systemHandle)
{
	//socket may derive blocking state from server socket: switch to blocking mode
#ifdef XPC_OS_WINDOWS
	u_long mode = 0;
	ioctlsocket(this->systemHandle.u64, FIONBIO, &mode);
#else
	int flags = fcntl(this->systemHandle.i32, F_GETFL, 0);
	flags = (flags & ~O_NONBLOCK);
	fcntl(this->systemHandle.i32, F_SETFL, flags);
#endif

	this->inputStream = new _stdxx_::TCPSocketInputStream(systemHandle);
	this->outputStream = new _stdxx_::TCPSocketOutputStream(systemHandle);
}

//Destructor
TCPSocket::~TCPSocket()
{
	this->inputStream.Reset();
	this->outputStream.Reset();

	shutdown(this->systemHandle.i32, SHUT_RDWR);

#ifdef XPC_OS_WINDOWS
	closesocket(this->systemHandle.u64);
#else
	close(this->systemHandle.i32);
#endif
}

//Public methods
bool TCPSocket::WaitForData(uint64 timeOut) const
{
	pollfd fd = {};
	fd.fd = this->systemHandle.i32;
	fd.events = POLLIN;

	//timeout
	int timeOut32Bit;
	if(timeOut == Unsigned<uint64>::Max())
		timeOut32Bit = -1; //forever
	else if(timeOut > Signed<int32>::Max())
		timeOut32Bit = Signed<int32>::Max();
	else
		timeOut32Bit = static_cast<int>(timeOut);

	//wait
#ifdef XPC_OS_WINDOWS
	NOT_IMPLEMENTED_ERROR;
	bool timedOut;
#else
	bool timedOut = poll(&fd, 1, timeOut32Bit) == 0;
#endif
	return !timedOut;
}
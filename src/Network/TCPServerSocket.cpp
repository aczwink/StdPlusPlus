/*
 * Copyright (c) 2019-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Network/TCPServerSocket.hpp>
//Global
#include <fcntl.h>
#ifndef XPC_OS_WINDOWS
#include <errno.h>
#include <poll.h>
#include <unistd.h>
#endif
//Local
#include <Std++/Network/IPv4Address.hpp>
#include <Std++/Signed.hpp>
#include <Std++/Errorhandling/Exceptions/AddressInUseException.hpp>
#include "Shared.hpp"
//Namespaces
using namespace StdXX;

//Local functions
static NetAddress* GetSocketBinding(const Variant& systemHandle, uint16& port)
{
	//TODO: currently only ipv4 is supported
	sockaddr_in ipv4;
	socklen_t addrlen = sizeof(ipv4);
	int result = getsockname(systemHandle.i32, (sockaddr*)&ipv4, &addrlen);
	ASSERT(result == 0, u8"If you see this, please report!");

	port = ipv4.sin_port;
	return new IPv4Address(ipv4.sin_addr.s_addr);
}

//Constructor
TCPServerSocket::TCPServerSocket(const NetAddress &netAddress, uint16 port)
{
	switch(netAddress.GetProtocolFamily())
	{
		case NetworkProtocolFamily::IPv4:
		{
			this->systemHandle.i32 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			//fill out address structure
			IPv4Address &v4Addr = (IPv4Address &)netAddress;
			sockaddr_in address = {0};
			address.sin_addr.s_addr = v4Addr.ToUInt32();
			address.sin_family = AF_INET;
			address.sin_port = htons(port);

			int ret = bind(this->systemHandle.i32, (sockaddr *)&address, sizeof(address));
			if(ret == -1)
			{
				switch (errno)
				{
					case EADDRINUSE:
						throw ErrorHandling::AddressInUseException(netAddress, port);
					default:
						NOT_IMPLEMENTED_ERROR;
				}
			}
		}
		break;
		default:
			NOT_IMPLEMENTED_ERROR;
	}

	//switch to non-blocking mode
#ifdef XPC_OS_WINDOWS
	u_long mode = 0;
	ioctlsocket(this->systemHandle.u64, FIONBIO, &mode);
#else
	int flags = fcntl(this->systemHandle.i32, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(this->systemHandle.i32, F_SETFL, flags);
#endif

	//enable listening
	listen(this->systemHandle.i32, 32);
}

//Destructor
TCPServerSocket::~TCPServerSocket()
{
#ifdef XPC_OS_WINDOWS
	shutdown(this->systemHandle.u64, SD_BOTH);
	closesocket(this->systemHandle.u64);
#else
	shutdown(this->systemHandle.i32, SHUT_RDWR);
	close(this->systemHandle.i32);
#endif
}

//Public methods
UniquePointer<NetAddress> TCPServerSocket::GetBoundAddress() const
{
	uint16 port;
	return GetSocketBinding(this->systemHandle, port);
}

uint16 TCPServerSocket::GetBoundPort() const
{
	uint16 port;
	delete GetSocketBinding(this->systemHandle, port);
	return ntohs(port);
}

UniquePointer<TCPSocket> TCPServerSocket::WaitForIncomingConnections(uint64 timeOut)
{
	pollfd fd = {};
	fd.fd = this->systemHandle.i32;
	fd.events = POLLIN;

	int timeOut32Bit;
	if(timeOut == Unsigned<uint64>::Max())
		timeOut32Bit = -1; //forever
	else if(timeOut > Signed<int32>::Max())
		timeOut32Bit = Signed<int32>::Max();
	else
		timeOut32Bit = static_cast<int>(timeOut);

	while(true)
	{
#ifdef XPC_OS_WINDOWS
		bool timedOut = false;
		NOT_IMPLEMENTED_ERROR;
#else
		bool timedOut = poll(&fd, 1, timeOut32Bit) == 0;
#endif

		if (timedOut)
			break;

		//try to accept
		int socket = accept(this->systemHandle.i32, nullptr, nullptr);
		if(socket == -1)
		{
			if(errno == EWOULDBLOCK)
				continue; //try again
			switch(errno)
			{
				case EAGAIN:
					continue; //try again
				case EBADF:
					NOT_IMPLEMENTED_ERROR; //TODO: error occured
				case ECONNABORTED:
					NOT_IMPLEMENTED_ERROR; //TODO: error occured
				case EINTR:
					NOT_IMPLEMENTED_ERROR; //TODO: error occured
				case EINVAL:
					NOT_IMPLEMENTED_ERROR; //TODO: error occured
				case EMFILE:
					NOT_IMPLEMENTED_ERROR; //TODO: error occured
				case ENFILE:
					NOT_IMPLEMENTED_ERROR; //TODO: error occured
				case ENOBUFS:
					NOT_IMPLEMENTED_ERROR; //TODO: error occured
				case ENOMEM:
					NOT_IMPLEMENTED_ERROR; //TODO: error occured
				case ENOTSOCK:
					NOT_IMPLEMENTED_ERROR; //TODO: error occured
				case EOPNOTSUPP:
					NOT_IMPLEMENTED_ERROR; //TODO: error occured
				case EPROTO:
					NOT_IMPLEMENTED_ERROR; //TODO: error occured
			}
		}

		Variant clientSystemHandle;
		clientSystemHandle.i32 = socket;
		return new TCPSocket(clientSystemHandle);
	}
	return nullptr;
}
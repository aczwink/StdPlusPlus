/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#ifdef XPC_OS_WINDOWS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <basetsd.h>
typedef SSIZE_T ssize_t;
#else
#include <netinet/in.h>
#include <sys/socket.h>
#endif
//Local
#include <Std++/Network/NetAddress.hpp>
//Namespaces
using namespace StdPlusPlus;

//Global functions
NetAddress *ParseNativeAddress(sockaddr *address, uint16 &port);
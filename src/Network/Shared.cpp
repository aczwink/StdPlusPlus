/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Definition header
#include "Shared.hpp"
//Local
#include <ACStdLib/Debug.h>
#include <ACStdLib/Memory.h>
#include <ACStdLib/Network/IPv4Address.hpp>

//Global functions
NetAddress *ParseNativeAddress(sockaddr *address, uint16 &port)
{
	switch(address->sa_family)
	{
		case AF_INET:
		{
			sockaddr_in *addrv4 = (sockaddr_in *) address;

			port = addrv4->sin_port;

			IPv4Address *result = new IPv4Address;
			MemCopy(result->rawAddress, &addrv4->sin_addr.s_addr, 4); //s_addr is in network order

			return result;
		}
	}

	return nullptr;
}
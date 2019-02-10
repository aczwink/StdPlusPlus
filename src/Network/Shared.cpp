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
//Definition header
#include "Shared.hpp"
//Local
#include <Std++/Debug.hpp>
#include <Std++/Memory.hpp>
#include <Std++/Network/IPv4Address.hpp>

//Global functions
NetAddress *ParseNativeAddress(sockaddr *address, uint16 &port)
{
	switch(address->sa_family)
	{
		case AF_INET:
		{
			sockaddr_in *addrv4 = (sockaddr_in *) address;

			port = ntohs(addrv4->sin_port);

			IPv4Address *result = new IPv4Address(addrv4->sin_addr.s_addr); //s_addr is in network order

			return result;
		}
	}

	return nullptr;
}
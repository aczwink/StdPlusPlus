/*
 * Copyright (c) 2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Network/NetAddress.hpp>
//Global
#include <netdb.h>
//Local
#include "Shared.hpp"
//Namespaces
using namespace StdXX;

//Class functions
String NetAddress::QueryHostName()
{
	char hostName[HOST_NAME_MAX];

	int result = gethostname(hostName, sizeof(hostName));
	ASSERT(result != -1, u8"gethostname failed");

	return String::CopyRawString(hostName);
}

DynamicArray<UniquePointer<NetAddress>> NetAddress::ResolveDomainName(const String& domainName)
{
	DynamicArray<UniquePointer<NetAddress>> result;

	hostent *pHost = gethostbyname((const char*)domainName.ToUTF8().GetRawZeroTerminatedData());
	ASSERT(pHost, u8"gethostbyname failed");

	for(uint32 i = 0; pHost->h_addr_list[i] != NULL; i++)
	{
		sockaddr_in socketAddress = {};
		MemCopy(&socketAddress.sin_addr, pHost->h_addr_list[i], pHost->h_length);
		socketAddress.sin_family = pHost->h_addrtype;

		result.Push(ParseNativeAddress((sockaddr*)&socketAddress));
	}

	return result;
}

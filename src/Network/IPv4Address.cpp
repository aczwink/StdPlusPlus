/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Network/IPv4Address.hpp>
//Local
#include "Shared.hpp"
//Namespaces
using namespace StdXX;

//Constructor
IPv4Address::IPv4Address(const String &string)
{
	auto parts = string.Split(u8".");

	for(uint8 i = 0; i < 4; i++)
		this->rawAddress[i] = static_cast<byte>(parts[i].ToUInt32());
}

//Operators
IPv4Address IPv4Address::operator+(uint32 delta) const
{
	return htonl(ntohl(this->ToUInt32()) + delta);
}

//Public methods
NetworkProtocolFamily IPv4Address::GetProtocolFamily() const
{
	return NetworkProtocolFamily::IPv4;
}

String IPv4Address::ToString() const
{
	return String::Number((uint64)this->rawAddress[0]) + u8"." + String::Number((uint64)this->rawAddress[1]) + u8"." + String::Number((uint64)this->rawAddress[2]) + u8"." + String::Number((uint64)this->rawAddress[3]);
}

uint32 IPv4Address::ToUInt32() const
{
	return htonl(this->ToHostOrderUInt32());
}

//Class functions
IPv4Address IPv4Address::FromHostEndianOrder(uint32 address)
{
	return htonl(address);
}

IPv4Address IPv4Address::GetAnyHostAddress()
{
	return IPv4Address(0);
}

IPv4Address IPv4Address::GetLoopbackAddress()
{
	return IPv4Address(127, 0, 0, 1);
}
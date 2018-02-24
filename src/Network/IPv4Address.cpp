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
//Class header
#include <Std++/Network/IPv4Address.hpp>
//Namespaces
using namespace StdPlusPlus;

//Public methods
NetworkProtocolFamily IPv4Address::GetProtocolFamily() const
{
	return NetworkProtocolFamily::IPv4;
}

String IPv4Address::ToString() const
{
	return String::Number((uint64)this->rawAddress[0]) + u8"." + String::Number((uint64)this->rawAddress[1]) + u8"." + String::Number((uint64)this->rawAddress[2]) + u8"." + String::Number((uint64)this->rawAddress[3]);
}

//Class functions
IPv4Address IPv4Address::GetLoopbackAddress()
{
	IPv4Address addr;

	addr.rawAddress[0] = 127;
	addr.rawAddress[1] = 0;
	addr.rawAddress[2] = 0;
	addr.rawAddress[3] = 1;

	return addr;
}
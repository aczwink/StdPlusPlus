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
#include <Std++/Network/DatagramSocket.hpp>
//Local
#include <Std++/Debug.hpp>
#include <Std++/Network/IPv4Address.hpp>
#include "Shared.hpp"
//Namespaces
using namespace StdPlusPlus;

//Constructor
DatagramSocket::DatagramSocket(const NetAddress &netAddress, uint16 port)
{
	switch(netAddress.GetProtocolFamily())
	{
		case NetworkProtocolFamily::IPv4:
		{
			this->systemHandle.i32 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

			//fill out address structure
			IPv4Address &v4Addr = (IPv4Address &)netAddress;
			sockaddr_in address = {0};
			address.sin_addr.s_addr = htonl(MAKE32_FROM4(v4Addr.rawAddress[0], v4Addr.rawAddress[1], v4Addr.rawAddress[2], v4Addr.rawAddress[3]));
			address.sin_family = AF_INET;
			address.sin_port = htons(port);

			bind(this->systemHandle.i32, (sockaddr *)&address, sizeof(address));
		}
		break;
		default:
			NOT_IMPLEMENTED_ERROR;
	}
}

//Public methods
bool DatagramSocket::ReceivePacket(Datagram &datagram)
{
	datagram.Reset();

	sockaddr senderAddress;
	socklen_t addressSize;
	addressSize = sizeof(senderAddress);
	ssize_t size = recvfrom(this->systemHandle.i32, (char *)datagram.GetBuffer(), datagram.GetBufferSize(), 0, &senderAddress, &addressSize);
	if(size >= 0)
	{
		datagram.sender = ParseNativeAddress(&senderAddress, datagram.senderPort);
		datagram.packetSize = (uint32) size;
		return true;
	}

	return false;
}
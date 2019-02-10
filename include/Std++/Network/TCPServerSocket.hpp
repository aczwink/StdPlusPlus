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
#pragma once
//Local
#include <Std++/SmartPointers/UniquePointer.hpp>
#include <Std++/Variant.hpp>
#include "NetAddress.hpp"
#include "TCPSocket.hpp"

namespace StdXX
{
	class TCPServerSocket
	{
	public:
		//Constructor
		/**
		 *
		 * @param netAddress
		 * @param port - use 0 to automatically select a random (free!) port
		 */
		TCPServerSocket(const NetAddress& netAddress, uint16 port = 0);

		//Destructor
		~TCPServerSocket();

		//Methods
		UniquePointer<NetAddress> GetBoundAddress() const;
		uint16 GetBoundPort() const;
		/**
		 *
		 * @param timeOut in milliseconds, Unsigned<uint64>::Max() means forever
		 * @return socket with new connection if one could be established, nullptr if the timeout elapsed before a new connection was established.
		 */
		UniquePointer<TCPSocket> WaitForIncomingConnections(uint64 timeOut);

	private:
		//Members
		Variant systemHandle;
	};
}
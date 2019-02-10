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
#pragma once
//Local
#include "NetAddress.hpp"

namespace StdXX
{
	class IPv4Address : public NetAddress
	{
	public:
		//Constructor
		/**
		 * Example: The IP 192.168.0.1 is created with the parameters set as following:
		 * msb: 192, smsb: 168, tmsb: 0, lsb: 1
		 * @param msb
		 * @param smsb
		 * @param tmsb
		 * @param lsb
		 */
		inline IPv4Address(byte msb, byte smsb, byte tmsb, byte lsb)
		{
			this->rawAddress[0] = msb;
			this->rawAddress[1] = smsb;
			this->rawAddress[2] = tmsb;
			this->rawAddress[3] = lsb;
		}

		/**
		 * @param address - Is expected to be in network order!
		 */
		inline IPv4Address(uint32 address)
		{
			MemCopy(rawAddress, &address, 4);
		}

		//Methods
		NetworkProtocolFamily GetProtocolFamily() const;
		String ToString() const;
		/**
		 * The return value will be in network byte order.
		 *
		 * @return
		 */
		uint32 ToUInt32() const;

		//Functions
		/**
		 * Returns the special address "0.0.0.0" that is used when wanting to listen on all IPv4 host addresses.
		 *
		 * @return
		 */
		static IPv4Address GetAnyHostAddress();

		/**
		 * Returns the loopback address for the communication point itself, which is 127.0.0.1 also known as "localhost"
		 *
		 * @return
		 */
		static IPv4Address GetLoopbackAddress();

	private:
		//Members
		/**
		 * In network order, i.e. \p index 0 has most significance.
		 *
		 * Example: The IP 192.168.0.1 is stored as follows:
		 * rawAddress[0] = 192
		 * rawAddress[1] = 168
		 * rawAddress[2] = 0
		 * rawAddress[3] = 1
		 *
		 */
		byte rawAddress[4];
	};
}
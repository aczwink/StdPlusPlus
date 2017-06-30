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
#pragma once
//Local
#include "NetAddress.hpp"

namespace ACStdLib
{
	class IPv4Address : public NetAddress
	{
	public:
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

		//Methods
		NetworkProtocolFamily GetProtocolFamily() const;
		String ToString() const;

		//Functions
		/**
		 * Returns the loopback address for the communication point itself, which is 127.0.0.1 also known as "localhost"
		 *
		 * @return
		 */
		static IPv4Address GetLoopbackAddress();
	};
}
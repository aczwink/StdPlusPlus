/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Errorhandling/Exception.hpp>
#include <Std++/Network/NetAddress.hpp>

namespace StdXX::ErrorHandling
{
	class AddressInUseException : public Exception
	{
	public:
		//Constructor
		inline AddressInUseException(const NetAddress &netAddress, uint16 port)
		{
			this->netAddressString = netAddress.ToString();
			this->port = port;
		}

		//Inline
		inline String Description() const
		{
			return u8"Address '" + this->netAddressString + u8":" + String::Number(this->port) + u8"' is already in use.";
		}

	private:
		//Members
		String netAddressString;
		uint16 port;
	};
}
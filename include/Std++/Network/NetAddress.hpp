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
#pragma once
//Local
#include <Std++/SmartPointers/UniquePointer.hpp>
#include "../Definitions.h"
#include "../Containers/Strings/String.hpp"

namespace StdXX
{
	enum class NetworkProtocolFamily
	{
		IPv4
	};

	class NetAddress
	{
	public:
		//Destructor
		virtual ~NetAddress(){}

		//Abstract
		virtual NetworkProtocolFamily GetProtocolFamily() const = 0;
		virtual String ToString() const = 0;

		//Functions
		static String QueryHostName();
		static DynamicArray<UniquePointer<NetAddress>> ResolveDomainName(const String& domainName);
	};
}
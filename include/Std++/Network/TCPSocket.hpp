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
//Local
#include <Std++/Variant.hpp>
#include <Std++/Streams/InputStream.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>

namespace StdXX
{
	class TCPSocket
	{
		friend class TCPServerSocket;

	public:
		//Destructor
		~TCPSocket();

		//Inline
		inline InputStream& GetInputStream()
		{
			return *this->inputStream;
		}

	private:
		//Constructor
		TCPSocket(const Variant& systemHandle); //called by TCPServerSocket

		//Members
		Variant systemHandle;
		UniquePointer<InputStream> inputStream;
	};
}
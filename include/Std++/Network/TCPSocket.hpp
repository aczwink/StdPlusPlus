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

		//Methods
		/**
		 * Wait until data arrives to be read from the socket.
		 *
		 * @param timeOut - in Milliseconds
		 * @return true if data is available, false if the timeout elapsed and no data arrived til then
		 */
		bool WaitForData(uint64 timeOut) const;

		//Inline
		inline InputStream& GetInputStream()
		{
			return *this->inputStream;
		}

		inline OutputStream& GetOutputStream()
		{
			return *this->outputStream;
		}

	private:
		//Constructor
		TCPSocket(const Variant& systemHandle); //called by TCPServerSocket

		//Members
		Variant systemHandle;
		UniquePointer<InputStream> inputStream;
		UniquePointer<OutputStream> outputStream;
	};
}
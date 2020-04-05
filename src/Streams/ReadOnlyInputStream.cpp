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
//Class header
#include <Std++/Streams/ReadOnlyInputStream.hpp>
//Local
#include <Std++/Mathematics.hpp>
#include <Std++/Constants.hpp>
//Namespaces
using namespace StdXX;

//Public methods
uint32 ReadOnlyInputStream::Skip(uint32 nBytes)
{
	byte buffer[c_io_blockSize];

	uint32 nSkipped = 0;
	while (nBytes)
	{
		if(this->IsAtEnd())
			break;

		uint32 nBytesToRead = Math::Min(uint32(sizeof(buffer)), nBytes);
		uint32 nBytesRead = this->ReadBytes(buffer, nBytesToRead);
		nBytes -= nBytesRead;
		nSkipped += nBytesRead;
	}

	return nSkipped;
}

/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/HashingInputStream.hpp>
//Lcoal
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;

//Public methods
bool HashingInputStream::IsAtEnd() const
{
	return this->inputStream.IsAtEnd();
}

uint32 HashingInputStream::ReadBytes(void * destination, uint32 count)
{
	uint32 nBytesRead = this->inputStream.ReadBytes(destination, count);
	this->hasher->Update((byte*)destination, nBytesRead);
	
	return nBytesRead;
}

uint32 HashingInputStream::Skip(uint32 nBytes)
{
	byte buffer[4096];

	uint32 nSkipped = 0;
	while (nBytes)
	{
		uint32 nBytesToRead = Math::Min(uint32(sizeof(buffer)), nBytes);
		uint32 nBytesRead = this->inputStream.ReadBytes(buffer, nBytesToRead);
		this->hasher->Update(buffer, nBytesRead);
		nBytes -= nBytesRead;
		nSkipped += nBytesRead;
	}

	return nSkipped;
}

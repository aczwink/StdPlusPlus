/*
* Copyright (c) 2018-2025 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/LimitedInputStream.hpp>
//Local
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;

//Public methods
uint32 LimitedInputStream::GetBytesAvailable() const
{
	return Math::Min(this->inputStream.GetBytesAvailable(), uint32(this->limit - this->processed));
}

bool LimitedInputStream::IsAtEnd() const
{
	return (this->processed == this->limit) || this->inputStream.IsAtEnd();
}

uint32 LimitedInputStream::ReadBytes(void * destination, uint32 count)
{
	count = Math::Min(count, uint32(this->limit - this->processed));
	uint32 bytesRead = this->inputStream.ReadBytes(destination, count);
	this->processed += bytesRead;

	return bytesRead;
}

uint32 LimitedInputStream::Skip(uint32 nBytes)
{
	nBytes = Math::Min(nBytes, uint32(this->limit - this->processed));
	uint32 nBytesSkipped = this->inputStream.Skip(nBytes);
	this->processed += nBytesSkipped;

	return nBytesSkipped;
}
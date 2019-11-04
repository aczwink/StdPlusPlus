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
//Class header
#include <Std++/Streams/CheckedInputStream.hpp>
//Local
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;

//Public methods
bool CheckedInputStream::Finish()
{
    if(!this->finished)
    {
        this->checkFunc->Finish();
        this->finished = true;
    }

    return this->expectedChecksum == this->checkFunc->GetChecksum();
}

uint32 CheckedInputStream::GetBytesAvailable() const
{
	return this->inputStream.GetBytesAvailable();
}

bool CheckedInputStream::IsAtEnd() const
{
	return this->inputStream.IsAtEnd();
}

uint32 CheckedInputStream::ReadBytes(void * destination, uint32 count)
{
	uint32 nBytesRead = this->inputStream.ReadBytes(destination, count);
	this->checkFunc->Update(destination, nBytesRead);

	if(!this->finished && this->inputStream.IsAtEnd())
    {
        bool matches = this->Finish();
        ASSERT(matches, u8"Wrong checksum! File probably corrupt. Report please!");
    }

	return nBytesRead;
}

uint32 CheckedInputStream::Skip(uint32 nBytes)
{
	byte buffer[4096];

	uint32 nSkipped = 0;
	while (nBytes)
	{
		uint32 nBytesToRead = Math::Min(uint32(sizeof(buffer)), nBytes);
		uint32 nBytesRead = this->ReadBytes(buffer, nBytesToRead);
		nBytes -= nBytesRead;
		nSkipped += nBytesRead;
	}

	return nSkipped;
}
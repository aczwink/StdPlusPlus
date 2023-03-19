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
//Class header
#include <Std++/Streams/ConcatenatedInputStream.hpp>
//Namespaces
using namespace StdXX;

//Public methods
bool ConcatenatedInputStream::IsAtEnd() const
{
	return (this->currentStream == (this->partialStreams.GetNumberOfElements() - 1)) and this->partialStreams.Last()->IsAtEnd();
}

uint32 ConcatenatedInputStream::ReadBytes(void* destination, uint32 count)
{
	uint8* dest = static_cast<uint8 *>(destination);
	while(count)
	{
		uint32 nBytesRead = this->partialStreams[this->currentStream]->ReadBytes(dest, count);
		count -= nBytesRead;

		if(this->partialStreams[this->currentStream]->IsAtEnd())
		{
			if(this->currentStream == this->partialStreams.GetNumberOfElements() - 1)
				break;

			this->currentStream++;
		}
	}

	return static_cast<uint32>(dest - static_cast<uint8 *>(destination));
}

uint32 ConcatenatedInputStream::Skip(uint32 nBytes)
{
	uint32 nBytesSkippedTotal = 0;
	while(nBytes)
	{
		uint32 nBytesSkipped = this->partialStreams[this->currentStream]->Skip(nBytes);
		nBytesSkippedTotal += nBytesSkipped;
		nBytes -= nBytesSkipped;

		if(this->partialStreams[this->currentStream]->IsAtEnd())
		{
			if(this->currentStream == this->partialStreams.GetNumberOfElements() - 1)
				break;

			this->currentStream++;
		}
	}

	return nBytesSkippedTotal;
}

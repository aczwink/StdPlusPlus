/*
 * Copyright (c) 2019-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Compression/DictionaryDecompressor.hpp>
//Namespaces
using namespace StdXX;

//Public methods
uint32 DictionaryDecompressor::GetBytesAvailable() const
{
	return this->nBytesInBuffer + this->inputStream.GetBytesAvailable();
}

bool DictionaryDecompressor::IsAtEnd() const
{
	return this->IsBufferEmpty() and this->inputStream.IsAtEnd();
}

uint32 DictionaryDecompressor::ReadBytes(void *destination, uint32 count)
{
	auto* dest = static_cast<uint8 *>(destination);
	while(count)
	{
		this->FillBufferIfEmpty();
		if(this->IsAtEnd())
			break;

		uint32 nBytesToCopy = Math::Min(count, (uint32)this->nBytesInBuffer);
		this->dictionary.Read(dest, this->nBytesInBuffer, nBytesToCopy);

		count -= nBytesToCopy;
		this->nBytesInBuffer -= nBytesToCopy;
		dest += nBytesToCopy;
	}
	this->FillBufferIfEmpty(); //always make sure to try to fill the buffer so that subsequent calls to "IsAtEnd" are always correct

	return static_cast<uint32>(dest - static_cast<uint8 *>(destination));
}

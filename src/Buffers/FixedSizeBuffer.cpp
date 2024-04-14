/*
* Copyright (c) 2019-2024 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Buffers/FixedSizeBuffer.hpp>
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;

//Local functions
static bool CheckIfMemoryMatches(const uint8* p1, const uint8* p2, uint32 size)
{
	for(uint32 i = 0; i < size; i++)
	{
		if(p1[i] != p2[i])
			return false;
	}
	return true;
}

//Public methods
uint32 FixedSizeBuffer::FindBytes(const uint8* bytes, uint32 size, uint32 startOffset, uint32 endOffset) const
{
	endOffset = Math::Min(endOffset, this->size);
	endOffset -= size;

	for(uint32 offset = startOffset; offset < endOffset; offset++)
	{
		if(CheckIfMemoryMatches(&this->data[offset], bytes, size))
			return offset;
	}
	return Unsigned<uint32>::Max();
}

uint32 FixedSizeBuffer::FindBytesReversed(const byte* bytes, uint32 size, uint32 startOffset)
{
	startOffset = Math::Min(startOffset, this->size - 1);
	if(size == 0)
		return startOffset;
	if(size > startOffset)
		return Unsigned<uint32>::Max();
	startOffset -= size;

	for(int64 offset = startOffset; offset >= 0; offset--)
	{
		if(CheckIfMemoryMatches(&this->data[offset], bytes, size))
			return offset;
	}

	return Unsigned<uint32>::Max();
}
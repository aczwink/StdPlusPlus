/*
 * Copyright (c) 2021-2024 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Compression/IndexedSlidingDictionary.hpp>
//Namespaces
using namespace StdXX;

//Public methods
IndexedSlidingDictionary::DictionaryMatch IndexedSlidingDictionary::FindLongestMatchAtSplitDistance(uint16 startDistance)
{
	DictionaryMatch bestMatch = {.distance = 0, .length = 0};

	if(startDistance < this->minBackRefLength)
		return bestMatch;

	uint8 buffer[4096];
	this->Read(buffer, startDistance, this->minBackRefLength);
	InlineByteString inlineByteString(buffer, this->minBackRefLength);

	if(!this->prefixTree.Contains(inlineByteString))
		return bestMatch;

	auto& indices = this->prefixTree.Get(inlineByteString);
	auto it = indices.end();
	--it;

	for(; it.IsValid(); --it)
	{
		uint32 distance = this->nBytesWritten - (*it);
		ASSERT(startDistance < distance, u8"Can't find a backreference into the future");

		if(distance > (this->maxDistanceDelta * 2))
		{
			//not reachable anymore
			it.Remove();
			continue;
		}

		uint32 relDistance = distance - startDistance;
		if(relDistance > this->maxDistanceDelta)
			continue;

		DictionaryMatch match = {.distance = (uint16)relDistance, .length = 0};
		match.length = this->ComputeMatchLength(startDistance, distance, startDistance);

		if(match.length > bestMatch.length)
			bestMatch = match;
	}

	return bestMatch;
}

void IndexedSlidingDictionary::IndexUpTo(uint16 distance)
{
	if(distance < this->minBackRefLength)
		return; //Can't index if not enough bytes in buffer
	uint32 targetCounter = this->nBytesWritten - distance;

	uint8 buffer[4096];
	for(uint32 counter = this->indexedBytesCounter; counter < targetCounter; counter++)
	{
		uint32 d = this->nBytesWritten - counter;
		this->Read(buffer, d, this->minBackRefLength);

		InlineByteString inlineByteString(buffer, this->minBackRefLength);
		this->prefixTree[inlineByteString].InsertTail(this->nBytesWritten - d);
	}

	this->indexedBytesCounter = Math::Max(targetCounter, this->indexedBytesCounter);
}

//Private methods
uint16 IndexedSlidingDictionary::ComputeMatchLength(uint32 d1, uint32 d2, uint16 length) const
{
	uint16 matchedLength = 0;

	uint32 maxReadLength = 0;
	const uint8* ptr1;
	const uint8* ptr2;
	for(uint16 i = 0; i < length; i++)
	{
		if(maxReadLength == 0)
		{
			uint32 idx1 = this->CalcBackOffsetIndex(d1 - i);
			uint32 idx2 = this->CalcBackOffsetIndex(d2 - i);
			uint32 len1 = this->CalcNumberOfBytesPossibleToRead(idx1);
			uint32 len2 = this->CalcNumberOfBytesPossibleToRead(idx2);
			maxReadLength = Math::Min(len1, len2);

			ptr1 = &this->Data()[idx1];
			ptr2 = &this->Data()[idx2];
		}

		if(*ptr1++ != *ptr2++)
			break;

		maxReadLength--;
		matchedLength++;
	}
	return matchedLength;
}
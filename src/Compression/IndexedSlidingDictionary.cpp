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
IndexedSlidingDictionary::DictionaryMatch IndexedSlidingDictionary::FindLongestMatchAtSplitDistance(uint32 startDistance, uint16 maxLength)
{
	ASSERT(startDistance <= this->IndexedDistanceFromTail(), u8"Can't find a backreference into the future");

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

	uint32 indexBeginOffset = (this->indexedBytesCounter < this->Size()) ? 0 : this->indexedBytesCounter - this->Size();
	uint32 indexEndOffset = this->indexedBytesCounter;

	for(; it.IsValid(); --it)
	{
		uint32 offset = *it;
		if(!Math::IsValueInInterval(offset, indexBeginOffset, indexEndOffset))
		{
			//not reachable anymore
			it.Remove();
			continue;
		}

		uint32 distanceFromTail = this->OffsetToDistanceFromTail(offset);
		uint32 relDistance = distanceFromTail - startDistance;
		if(relDistance > this->maxDistanceDelta)
			continue;

		DictionaryMatch match = {.distance = (uint16)relDistance, .length = 0};
		uint32 maxLengthToMatch = Math::Min(startDistance, (uint32)maxLength);
		match.length = this->ComputeMatchLength(startDistance, distanceFromTail, static_cast<uint16>(maxLengthToMatch));

		if(match.length > bestMatch.length)
			bestMatch = match;
	}

	return bestMatch;
}

void IndexedSlidingDictionary::IndexUpTo(uint32 distance)
{
	uint8 buffer[4096];
	while( (this->IndexedDistanceFromTail() > distance) and (this->IndexedDistanceFromTail() >= this->minBackRefLength) )
	{
		this->Read(buffer, this->IndexedDistanceFromTail(), this->minBackRefLength);

		InlineByteString inlineByteString(buffer, this->minBackRefLength);
		this->prefixTree[inlineByteString].InsertTail(this->indexedBytesCounter);

		this->indexedBytesCounter++;
	}
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
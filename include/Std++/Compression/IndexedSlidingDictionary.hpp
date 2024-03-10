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
#pragma once
//Local
#include <Std++/Containers/Array/StaticArray.hpp>
#include <Std++/Containers/LinkedList/LinkedList.hpp>
#include <Std++/Containers/PrefixTree.hpp>
#include "SlidingDictionary.hpp"

namespace StdXX
{
	class IndexedSlidingDictionary : private SlidingDictionary
	{
		struct InlineByteString
		{
			const uint8* buffer;
			uint8 length;

			inline InlineByteString(const uint8* buffer, uint8 length) : buffer(buffer), length(length)
			{
			}

			inline const uint8* begin() const
			{
				return this->buffer;
			}

			inline const uint8* end() const
			{
				return this->buffer + this->length;
			}
		};
	public:
		struct DictionaryMatch
		{
			uint16 distance;
			uint16 length;
		};

		//Constructor
		/**
		 * @param size
		 * @param minBackRefLength
		 * @param maxDistanceDelta The maximum relative difference that can be encoded
		 */
		inline IndexedSlidingDictionary(uint32 size, uint8 minBackRefLength, uint16 maxDistanceDelta) : SlidingDictionary(size)
		{
			this->indexedBytesCounter = 0;
			this->nBytesWritten = 0;
			this->minBackRefLength = minBackRefLength;
			this->maxDistanceDelta = maxDistanceDelta;
		}

		//Methods
		/**
		 * This method treats the dictionary as being split into two parts.
		 * Anything before (i.e. bytes written earlier) the split is considered already encoded data,
		 * while anything after it is considered data to be encoded.
		 * Thus the method tries to find the longest word starting at \p splitDistance anywhere in the already encoded data.
		 * The length of the matched word can actually overlap the second half, but the distance is strictly matched against the first half.
		 *
		 * This enables this dictionary to be used as a backreference source, as well as a buffer for data that is to be matched for backreferences.
		 *
		 * @param splitDistance Distance that marks the beginning of word that should be matched against.
		 * @return The closest longest match. The distance will be relative to \p splitDistance
		 */
		DictionaryMatch FindLongestMatchAtSplitDistance(uint16 splitDistance);
		void IndexUpTo(uint16 distance);
		using RingBuffer::Read;

		//Inline
		inline void Append(uint8 byte)
		{
			RingBuffer::Append(byte);
			this->nBytesWritten++;
		}

	private:
		//Members
		uint8 minBackRefLength;
		uint16 maxDistanceDelta;
		uint32 indexedBytesCounter;
		uint32 nBytesWritten;
		PrefixTree<uint8, InlineByteString, LinkedList<uint32>> prefixTree;

		//Methods
		uint16 ComputeMatchLength(uint32 d1, uint32 d2, uint16 length) const;

		//Inline
	};
}
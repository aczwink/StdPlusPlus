/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Compression/RedundancyBasedCompressor.hpp>
//Namespaces
using namespace StdXX;

//Constructor
RedundancyBasedCompressor::RedundancyBasedCompressor(OutputStream &outputStream, RedundancyBasedCompressor::Config config) : Compressor(outputStream), config(config),
	dictionary(
			config.maxDistance + (config.maxBackrefLength * 2_u16), //to find longest backreferences we need to include full distance range + the length that a backref may have
			//since this class also enables finding overlapping backreferences we need "a bit" more buffer space, hence we take for simplicity reasons the double of the length
			config.minBackrefLength, config.maxDistance)
{
	this->nUnprocessedBytesInDictionary = 0;

	this->targetUnprocessedBytesCount = config.maxBackrefLength * 2_u32; //at the "split point" of the dictionary, there will be config.maxDistance bytes of history,
	// config.maxBackrefLength unprocessed bytes for matching and again config.maxBackrefLength for overlapping backreferences

	this->InitDictionary();
}

//Public methods
void RedundancyBasedCompressor::Finalize()
{
	Compressor::Finalize();

	while(this->nUnprocessedBytesInDictionary)
		this->EmitBlock();
}

void RedundancyBasedCompressor::Flush()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

uint32 RedundancyBasedCompressor::WriteBytes(const void *source, uint32 size)
{
	const auto * src = static_cast<const uint8 *>(source);
	while(size)
	{
		uint32 space = this->targetUnprocessedBytesCount - this->nUnprocessedBytesInDictionary;
		uint32 nBytesToWrite = Math::Min(space, size);
		for(uint32 i = 0; i < nBytesToWrite; i++)
			this->dictionary.Append(*src++);

		this->nUnprocessedBytesInDictionary += nBytesToWrite;
		size -= nBytesToWrite;

		if(this->nUnprocessedBytesInDictionary == this->targetUnprocessedBytesCount)
			this->EmitBlock();
	}

	uint32 totalWrittenBytes = static_cast<uint32>(src - static_cast<const uint8 *>(source));
	return totalWrittenBytes;
}

//Private methods
void RedundancyBasedCompressor::EmitBlock()
{
	auto match = this->dictionary.FindLongestMatchAtSplitDistance(this->nUnprocessedBytesInDictionary, this->config.maxBackrefLength);

	if(match.length >= this->config.minBackrefLength)
	{
		int32 checkResult = this->CheckPrimaryBackreference(match.distance, match.length);
		if(checkResult >= 0)
		{
			if(!this->TryFindBetterMatch(match, checkResult))
			{
				this->EmitBackreference(match.distance, match.length);
				this->nUnprocessedBytesInDictionary -= match.length;
				this->UpdateIndex();
				return;
			}
		}
	}

	//only emit always a single byte to allow searching backreferences at the next byte
	this->EmitDirectByte();
	this->nUnprocessedBytesInDictionary -= 1;
	this->UpdateIndex();
}

bool RedundancyBasedCompressor::TryFindBetterMatch(const IndexedSlidingDictionary::DictionaryMatch& startMatch, uint16 length)
{
	length = Math::Min(length, startMatch.length);
	length++; //since we start at 1
	for(uint16 i = 1; i < length; i++)
	{
		auto match = this->dictionary.FindLongestMatchAtSplitDistance(this->nUnprocessedBytesInDictionary - i, this->config.maxBackrefLength);
		if(match.length > startMatch.length)
		{
			if(this->CheckSecondaryBackreference(startMatch.distance, startMatch.length, match.distance, match.length, i))
				return true;
		}
	}
	return false;
}

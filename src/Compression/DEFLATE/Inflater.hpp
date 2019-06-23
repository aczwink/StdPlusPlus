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
//Local
#include <Std++/Compression/Decompressor.hpp>
#include <Std++/Compression/HuffmanDecoder.hpp>
#include <Std++/Containers/FIFOBuffer.hpp>
#include <Std++/Streams/Bitstreams/BitInputStreamBitReversed.hpp>
#include "../SlidingDictionary.hpp"

//Implemented as of RFC 1951
namespace _stdxx_
{
	class Inflater : public StdXX::Decompressor
	{
	public:
		//Constructor
		Inflater(InputStream& inputStream);

		//Methods
		uint32 GetBytesAvailable() const override;
		bool IsAtEnd() const override;
		uint32 ReadBytes(void *destination, uint32 count) override;
		uint32 Skip(uint32 nBytes) override;

	private:
		//Members
		bool lastBlock;
		StdXX::FIFOBuffer buffer;
		StdXX::BitInputStreamBitReversed bitInput;
		SlidingDictionary dictionary;

		//Methods
		uint16 DecodeDistance(uint16 distance);
		uint16 DecodeLength(uint16 literalLength);
		void DecompressBlock();
		void DecompressHuffmanBlock(StdXX::HuffmanDecoder &refLiteralLengthDecoder, StdXX::HuffmanDecoder &refDistanceDecoder);
		void DecodeHuffmanTrees(StdXX::HuffmanDecoder *&refpLiteralLengthDecoder, StdXX::HuffmanDecoder *&refpDistanceDecoder);
		void DecompressUncompressedBlock();
	};
}
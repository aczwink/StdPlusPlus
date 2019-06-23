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
#include "Inflater.hpp"
//Local
#include <Std++/Mathematics.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Global variables
HuffmanDecoder* g_fixedLiteralLengthDecoder = nullptr;
HuffmanDecoder* g_fixedDistanceDecoder = nullptr;

//Constructor
Inflater::Inflater(InputStream &inputStream) : Decompressor(inputStream), dictionary(32 * KiB), bitInput(inputStream),
	lastBlock(false)
{
	if(g_fixedLiteralLengthDecoder == nullptr)
	{
		uint16 i;
		uint8 bitLengths[288];

		//3.2.6. fixed canonical huffman tables

		for(i = 0; i <= 143; i++)
			bitLengths[i] = 8;
		for(; i <= 255; i++)
			bitLengths[i] = 9;
		for(; i <= 279; i++)
			bitLengths[i] = 7;
		for(; i <= 287; i++)
			bitLengths[i] = 8;

		g_fixedLiteralLengthDecoder = new HuffmanDecoder(288);
		g_fixedLiteralLengthDecoder->ConstructCanonical(bitLengths, 288);

		for(i = 0; i < 32; i++)
			bitLengths[i] = 5;

		g_fixedDistanceDecoder = new HuffmanDecoder(32);
		g_fixedDistanceDecoder->ConstructCanonical(bitLengths, 32);
	}
}

//Public methods
uint32 _stdxx_::Inflater::GetBytesAvailable() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

bool Inflater::IsAtEnd() const
{
	return this->lastBlock && this->buffer.IsEmpty();
}

uint32 Inflater::ReadBytes(void *destination, uint32 count)
{
	uint8* dest = static_cast<uint8 *>(destination);
	while(count)
	{
		if(this->buffer.IsEmpty())
		{
			if(this->lastBlock)
				break;
			this->DecompressBlock();
		}

		uint32 left = Math::Min(count, this->buffer.GetRemainingBytes());
		uint32 nBytesRead = this->buffer.ReadBytes(dest, left);
		dest += nBytesRead;
		count -= nBytesRead;
	}

	return dest - (uint8*)destination;
}

uint32 _stdxx_::Inflater::Skip(uint32 nBytes)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

//Private methods
uint16 Inflater::DecodeDistance(uint16 distance)
{
	uint8 nExtraBits;

	//3.2.5

	if(distance <= 3)
		return distance + 1;

	nExtraBits = distance / 2 - 1;
	distance = (distance % 2) + 2; //2 entries per bit length, but +2 because we want only bit lengths > 0

	//1 is minimum value of length
	return (distance << nExtraBits) + (uint16)this->bitInput.Read(nExtraBits) + 1;
}

uint16 Inflater::DecodeLength(uint16 literalLength)
{
	//3.2.5

	if(literalLength <= 264)
		return literalLength - 254;
	else if(literalLength <= 284)
	{
		uint8 nExtraBits;

		nExtraBits = (literalLength - 261) / 4;
		literalLength = ((literalLength - 265) % 4 + 4); //4 entries per bit length, but +4 because we want only bit lengths > 0

		//3 is minimum value of length
		return (literalLength << nExtraBits) + (uint16)this->bitInput.Read(nExtraBits) + 3;
	}

	return 258;
}

void Inflater::DecompressBlock()
{
	uint8 blockType;

	//3.2.3.

	if(!this->lastBlock)
	{
		this->lastBlock = bitInput.Read(1) != 0;
		blockType = (uint8)bitInput.Read(2);

		switch(blockType)
		{
			case 0:
			{
				//skip unaligned bits
				bitInput.Skip(8 - (bitInput.GetReadBitsCount() % 8));

				this->DecompressUncompressedBlock();
			}
				break;
			case 1:
			{
				this->DecompressHuffmanBlock(*g_fixedLiteralLengthDecoder, *g_fixedDistanceDecoder);
			}
				break;
			case 2:
			{
				HuffmanDecoder *pLiteralLengthDecoder, *pDistanceDecoder;

				this->DecodeHuffmanTrees(pLiteralLengthDecoder, pDistanceDecoder);
				this->DecompressHuffmanBlock(*pLiteralLengthDecoder, *pDistanceDecoder);

				delete pLiteralLengthDecoder;
				delete pDistanceDecoder;
			}
				break;
			default:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	}
}

void Inflater::DecompressHuffmanBlock(HuffmanDecoder &refLiteralLengthDecoder, HuffmanDecoder &refDistanceDecoder)
{
	uint16 literalLength;

	//3.2.3

	while(true)
	{
		literalLength = refLiteralLengthDecoder.Decode(this->bitInput);
		if(literalLength < 256)
		{
			uint8 literalLengthByte = (uint8)literalLength;
			this->buffer.WriteBytes(&literalLengthByte, 1);
			this->dictionary.Append((uint8)literalLength);
		}
		else if(literalLength == 256)
		{
			break;
		}
		else
		{
			uint16 distance;

			//3.2.5

			literalLength = this->DecodeLength(literalLength);
			distance = (uint16)refDistanceDecoder.Decode(this->bitInput);
			distance = this->DecodeDistance(distance);

			this->dictionary.Copy(distance, literalLength, this->buffer);
		}
	}
}

void Inflater::DecodeHuffmanTrees(HuffmanDecoder *&refpLiteralLengthDecoder, HuffmanDecoder *&refpDistanceDecoder)
{
	uint8 nDistanceCodes, nCodeLengthCodes, codeLength, runValue, runLength;
	uint16 nLiteralLengthCodes, i;
	uint8 codeLengthCodeLengths[19];
	uint8 codeLenghts[318];

	HuffmanDecoder codeLengthDecoder(19);

	//3.2.7

	static const uint8 codeLengthsOrder[19] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};

	nLiteralLengthCodes = (uint16)this->bitInput.Read(5) + 257;
	nDistanceCodes = (uint8)this->bitInput.Read(5) + 1;
	nCodeLengthCodes = (uint8)this->bitInput.Read(4) + 4;

	//decode code length alphabet for code lengths
	for(i = 0; i < nCodeLengthCodes; i++)
		codeLengthCodeLengths[codeLengthsOrder[i]] = (uint8)this->bitInput.Read(3);
	for(; i < 19; i++)
		codeLengthCodeLengths[codeLengthsOrder[i]] = 0;

	codeLengthDecoder.ConstructCanonical(codeLengthCodeLengths, 19);

	//decode code lengths (HLIT and HDIST many)
	runLength = 0;
	for(i = 0; i < nLiteralLengthCodes + nDistanceCodes;)
	{
		if(runLength)
		{
			codeLenghts[i++] = runValue;
			runLength--;
			continue;
		}

		codeLength = (uint8)codeLengthDecoder.Decode(this->bitInput);
		switch(codeLength)
		{
			case 16:
			{
				runLength = (uint8)this->bitInput.Read(2) + 3;
			}
				break;
			case 17:
			{
				runLength = (uint8)this->bitInput.Read(3) + 3;
				runValue = 0;
			}
				break;
			case 18:
			{
				runLength = (uint8)this->bitInput.Read(7) + 11;
				runValue = 0;
			}
				break;
			default: //<= 15
				//direct mapped to code length
				codeLenghts[i++] = codeLength;
				runValue = codeLength;
		}
	}

	refpLiteralLengthDecoder = new HuffmanDecoder(nLiteralLengthCodes);
	refpLiteralLengthDecoder->ConstructCanonical(codeLenghts, nLiteralLengthCodes);

	refpDistanceDecoder = new HuffmanDecoder(nDistanceCodes);
	refpDistanceDecoder->ConstructCanonical(&codeLenghts[nLiteralLengthCodes], nDistanceCodes);
}

void Inflater::DecompressUncompressedBlock()
{
	//3.2.3 and 3.2.4

	DataReader dataReader(false, this->inputStream);

	uint16 length = dataReader.ReadUInt16();
	this->inputStream.Skip(2); //ones complement of 'len' for error safety

	while(length--)
	{
		byte b = dataReader.ReadByte();

		this->dictionary.Append(b);
		this->buffer.WriteBytes(&b, 1);
	}
}
/*
 * Copyright (c) 2018-2019 Amir Czwink (amir130@hotmail.de)
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
#include "LZMADecompressor.hpp"
//Local
#include <Std++/Debug.hpp>
#include <Std++/Mathematics.hpp>
#include <Std++/Memory.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

void *SzAlloc(ISzAllocPtr p, size_t size) { return MemAlloc(size); }
void SzFree(ISzAllocPtr p, void *address) { MemFree(address); }
const ISzAlloc g_Alloc = { SzAlloc, SzFree };

//Constructors
LZMADecompressor::LZMADecompressor(InputStream& inputStream) : Decompressor(inputStream)
{
	LzmaDec_Construct(&this->state);

	unsigned char header[LZMA_PROPS_SIZE + 8];
	this->inputStream.ReadBytes(header, sizeof(header));

	this->uncompressedSize = 0;
	for (uint8 i = 0; i < 8; i++)
		this->uncompressedSize += (UInt64)header[LZMA_PROPS_SIZE + i] << (i * 8);
	this->leftSize = this->uncompressedSize;

	SRes res = LzmaDec_Allocate(&this->state, header, LZMA_PROPS_SIZE, &g_Alloc);
	ASSERT(res == SZ_OK, u8"Report this please!");

	LzmaDec_Init(&this->state);
}

LZMADecompressor::LZMADecompressor(InputStream& inputStream, uint64 uncompressedSize, const byte* header, uint32 headerSize) : Decompressor(inputStream), uncompressedSize(uncompressedSize), leftSize(uncompressedSize)
{
	LzmaDec_Construct(&this->state);

	ASSERT(headerSize == 5, u8"Report this please!");
	SRes res = LzmaDec_Allocate(&this->state, header, LZMA_PROPS_SIZE, &g_Alloc);
	ASSERT(res == SZ_OK, u8"Report this please!");

	LzmaDec_Init(&this->state);
}

//Destructor
LZMADecompressor::~LZMADecompressor()
{
	LzmaDec_Free(&this->state, &g_Alloc);
}

//Public methods
bool LZMADecompressor::IsAtEnd() const
{
	return this->buffer.IsAtEnd() && this->inputStream.IsAtEnd();
}

uint32 LZMADecompressor::ReadBytes(void * destination, uint32 count)
{
	uint32 total = 0;

	byte* dest = (byte*)destination;
	while (count)
	{
		if (this->IsAtEnd())
			break;

		if (this->buffer.IsEmpty())
			this->Decode(true);
		
		uint32 nBytesToRead = Math::Min(count, this->buffer.GetRemainingBytes());
		uint32 nBytesRead = this->buffer.ReadBytes(dest, nBytesToRead);

		dest += nBytesRead;
		total += nBytesRead;
		count -= nBytesRead;
	}

	return total;
}

uint32 LZMADecompressor::Skip(uint32 nBytes)
{
	uint32 total = 0;
	while (nBytes)
	{
		if (this->IsAtEnd())
			break;

		if (this->buffer.IsEmpty())
			this->Decode(false);

		uint32 nBytesToSkip = Math::Min(nBytes, this->buffer.GetRemainingBytes());
		uint32 nBytesSkipped = this->buffer.Skip(nBytesToSkip);

		total += nBytesSkipped;
		nBytes -= nBytesSkipped;
	}

	return total;
}

//Private methods
void LZMADecompressor::Decode(bool write, ELzmaFinishMode finishMode)
{
#define IN_BUF_SIZE (1 << 16)
#define OUT_BUF_SIZE (1 << 16)
	Byte inBuf[IN_BUF_SIZE];
	Byte outBuf[OUT_BUF_SIZE];

	size_t inPos = 0, outPos = 0;

	uint32 inSize = this->inputStream.ReadBytes(inBuf, IN_BUF_SIZE);

	while((inPos != inSize) || (finishMode != LZMA_FINISH_ANY))
	{
		SizeT inProcessed = inSize - inPos;
		SizeT outProcessed = OUT_BUF_SIZE - outPos;

		if (this->IsUncompressedSizeKnown() && outProcessed > this->leftSize)
		{
			outProcessed = (SizeT)this->leftSize;
			finishMode = LZMA_FINISH_END;
		}

		ELzmaStatus status;
		SRes res = LzmaDec_DecodeToBuf(&this->state, outBuf + outPos, &outProcessed, inBuf + inPos, &inProcessed, finishMode, &status);
		inPos += inProcessed;
		outPos += outProcessed;
		this->leftSize -= outProcessed;

		if(write)
			this->buffer.WriteBytes(outBuf, outPos);
		outPos = 0;

		if (res != SZ_OK)
		{
			NOT_IMPLEMENTED_ERROR; //TODO: handle error
		}

		if (inProcessed == 0 && outProcessed == 0)
		{
			if (this->IsUncompressedSizeKnown() || status != LZMA_STATUS_FINISHED_WITH_MARK)
			{
				NOT_IMPLEMENTED_ERROR; //TODO: handle error
			}
			NOT_IMPLEMENTED_ERROR; //TODO: handle error
		}
	}
}
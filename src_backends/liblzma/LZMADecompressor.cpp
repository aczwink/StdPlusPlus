/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Mathematics.hpp>
#include <Std++/Memory.hpp>
#include "LZMADecompressor.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Constructor
LZMADecompressor::LZMADecompressor(InputStream& inputStream) : Decompressor(inputStream)
{
	this->lzmaStream = LZMA_STREAM_INIT;

	lzma_ret ret = lzma_alone_decoder(&this->lzmaStream, UINT64_MAX);
	ASSERT(ret == LZMA_OK, u8"REPORT THIS PLEASE!");

	this->streamEnd = false;
	this->lzmaStream.avail_in = 0;
	this->nBytesInOutBuffer = 0;
}

//Destructor
LZMADecompressor::~LZMADecompressor()
{
	lzma_end(&this->lzmaStream);
}

//Public methods
uint32 LZMADecompressor::GetBytesAvailable() const
{
	return this->nBytesInOutBuffer;
}

bool LZMADecompressor::IsAtEnd() const
{
	return (this->nBytesInOutBuffer == 0) && (
			((this->lzmaStream.avail_in == 0) && this->inputStream.IsAtEnd()) || this->streamEnd
	);
}

uint32 LZMADecompressor::ReadBytes(void *destination, uint32 count)
{
	uint8* dest = static_cast<uint8 *>(destination);
	while(count)
	{
		if (this->IsAtEnd())
			break;
		if(this->nBytesInOutBuffer == 0)
			this->DecompressNextBlock();

		uint32 nBytesToCopy = Math::Min(count, this->nBytesInOutBuffer);
		MemCopy(dest, this->outBufferCurrent, nBytesToCopy);

		this->outBufferCurrent += nBytesToCopy;
		this->nBytesInOutBuffer -= nBytesToCopy;

		dest += nBytesToCopy;
		count -= nBytesToCopy;
	}

	return dest - (uint8*)destination;
}

uint32 _stdxx_::LZMADecompressor::Skip(uint32 nBytes)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

//Private methods
void LZMADecompressor::DecompressNextBlock()
{
	lzma_action action = LZMA_RUN;
	if(this->lzmaStream.avail_in == 0)
	{
		this->lzmaStream.avail_in = this->inputStream.ReadBytes(this->inBuffer, sizeof(this->inBuffer));
		this->lzmaStream.next_in = this->inBuffer;
	}
	if(this->inputStream.IsAtEnd())
		action = LZMA_FINISH;

	this->lzmaStream.avail_out = sizeof(this->outBuffer);
	this->lzmaStream.next_out = this->outBuffer;

	lzma_ret ret = lzma_code(&this->lzmaStream, action);
	switch(ret)
	{
		case LZMA_OK:
			break;
		case LZMA_STREAM_END:
			this->streamEnd = true;
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	this->outBufferCurrent = this->outBuffer;
	this->nBytesInOutBuffer = sizeof(this->outBuffer) - this->lzmaStream.avail_out;
}

/*
 * Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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
#include "LZMACompressor.hpp"
//Global
#include <lzma.h>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Constructor
LZMACompressor::LZMACompressor(OutputStream &outputStream, StdXX::Optional<uint8> compressionLevel) : Compressor(outputStream)
{
	this->lzmaStream = LZMA_STREAM_INIT;

	uint8 preset = (uint8) (compressionLevel.HasValue() ? *compressionLevel : LZMA_PRESET_DEFAULT);

	//init encoder
	lzma_options_lzma options_lzma;
	lzma_bool ok = lzma_lzma_preset(&options_lzma, preset);
	ASSERT(ok == false, u8"TODO: report this please!");

	lzma_ret ret = lzma_alone_encoder(&this->lzmaStream, &options_lzma); //lzma with properties, dictionary size and uncompressed size as header
	ASSERT(ret == LZMA_OK, u8"TODO: report this please!");

	this->lzmaStream.next_out = this->outputBuffer;
	this->lzmaStream.avail_out = sizeof(this->outputBuffer);
}

//Destructor
LZMACompressor::~LZMACompressor()
{
	lzma_end(&this->lzmaStream);
}

//Public methods
void LZMACompressor::Finalize()
{
	while(this->CompressBlock(LZMA_FINISH)); //we need to issue this multiple times, since the output buffer might be full
	this->Flush();
}

void LZMACompressor::Flush()
{
	uint32 size = sizeof(this->outputBuffer) - this->lzmaStream.avail_out;
	this->outputStream.WriteBytes(this->outputBuffer, size);
	this->outputStream.Flush();

	//give it again the full buffer
	this->lzmaStream.next_out = this->outputBuffer;
	this->lzmaStream.avail_out = sizeof(this->outputBuffer);
}

uint32 LZMACompressor::WriteBytes(const void *source, uint32 size)
{
	//give buffer to liblzma
	this->lzmaStream.next_in = (byte *) source;
	this->lzmaStream.avail_in = size;

	while(this->lzmaStream.avail_in > 0) //continue until input is fully consumed
		this->CompressBlock(LZMA_RUN);

	return size;
}

//Private methods
bool LZMACompressor::CompressBlock(lzma_action action)
{
	//compress
	lzma_ret ret = lzma_code(&this->lzmaStream, action);

	//write compressed
	if(this->lzmaStream.avail_out == 0)
	{
		this->outputStream.WriteBytes(this->outputBuffer, sizeof(this->outputBuffer));

		//give it again the full buffer
		this->lzmaStream.next_out = this->outputBuffer;
		this->lzmaStream.avail_out = sizeof(this->outputBuffer);
	}

	//check status
	switch(ret)
	{
		case LZMA_OK: //ok
			break;
		case LZMA_STREAM_END:
			return false;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	return true;
}
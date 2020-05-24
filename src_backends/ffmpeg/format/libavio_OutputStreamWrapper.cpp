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
#include "libavio_OutputStreamWrapper.hpp"
//Local
#include <Std++/Constants.hpp>
#include <Std++/Debug.hpp>
#include <Std++/Signed.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Local functions
static int write_packet(void *opaque, uint8_t *buf, int buf_size)
{
	libavio_OutputStreamWrapper* wrapper = (libavio_OutputStreamWrapper *)opaque;
	return wrapper->WritePacket(buf, buf_size);
}

static int64_t seek(void *opaque, int64_t offset, int whence)
{
	libavio_OutputStreamWrapper* wrapper = (libavio_OutputStreamWrapper *)opaque;

	if(whence & AVSEEK_SIZE)
		return Signed<int64>::Max();

	ASSERT(whence == 0, u8"TODO: IMPLEMENT THIS");
	return wrapper->SeekTo(offset);
}

//Constructor
libavio_OutputStreamWrapper::libavio_OutputStreamWrapper(SeekableOutputStream &seekableOutputStream)
	: seekableOutputStream(seekableOutputStream)
{
	this->avio_ctx_buffer = static_cast<uint8 *>(av_malloc(c_io_blockSize));
	this->avio_ctx = avio_alloc_context(avio_ctx_buffer, c_io_blockSize, 1, this, nullptr, write_packet, &seek);
}

//Destructor
libavio_OutputStreamWrapper::~libavio_OutputStreamWrapper()
{
	if (this->avio_ctx)
		av_freep(&this->avio_ctx->buffer);
	avio_context_free(&this->avio_ctx);
}
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
#include "ExtZLIBInflater.hpp"
//Local
#include <Std++/Mathematics.hpp>
#include <Std++/Memory.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Constructor
ExtZLIBInflater::ExtZLIBInflater(InputStream& inputStream) : Decompressor(inputStream)
{
    this->strm.zalloc = Z_NULL;
    this->strm.zfree = Z_NULL;
    this->strm.opaque = Z_NULL;
    this->strm.avail_in = 0;
    this->strm.next_in = Z_NULL;
    int ret = inflateInit2(&this->strm, -15);
    ASSERT(ret == Z_OK, u8"REPORT THIS PLEASE!");

    this->nBytesInInBuffer = 0;
    this->nBytesInOutBuffer = 0;
}

//Destructor
ExtZLIBInflater::~ExtZLIBInflater()
{
    inflateEnd(&this->strm);
}

//Public methods
uint32 _stdxx_::ExtZLIBInflater::GetBytesAvailable() const
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return 0;
}

bool ExtZLIBInflater::IsAtEnd() const
{
    return (this->nBytesInInBuffer == 0) && (this->nBytesInOutBuffer == 0) && this->inputStream.IsAtEnd();
}

uint32 ExtZLIBInflater::ReadBytes(void *destination, uint32 count)
{
    uint8* dest = static_cast<uint8 *>(destination);
    while(count)
    {
        if(this->IsAtEnd())
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

uint32 _stdxx_::ExtZLIBInflater::Skip(uint32 nBytes)
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return 0;
}

//Private methods
void ExtZLIBInflater::DecompressNextBlock()
{
    if(this->nBytesInInBuffer == 0)
    {
        this->nBytesInInBuffer = this->inputStream.ReadBytes(this->inBuffer, c_bufferSize);
        this->inBufferCurrent = this->inBuffer;
    }

    this->strm.avail_in = this->nBytesInInBuffer;
    this->strm.next_in = this->inBufferCurrent;

    this->strm.avail_out = c_bufferSize;
    this->strm.next_out = this->outBuffer;

    int ret = inflate(&this->strm, Z_NO_FLUSH);
    ASSERT(ret != Z_STREAM_ERROR, u8"REPORT THIS PLEASE!");
    switch(ret)
    {
        case Z_NEED_DICT:
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            NOT_IMPLEMENTED_ERROR; //TODO: implement me
    }

    this->outBufferCurrent = this->outBuffer;
    this->nBytesInOutBuffer = c_bufferSize - this->strm.avail_out;

    uint32 nBytesConsumed = this->nBytesInInBuffer - this->strm.avail_in;
    this->nBytesInInBuffer -= nBytesConsumed;
    this->inBufferCurrent += nBytesConsumed;
}

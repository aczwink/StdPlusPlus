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
#include "ZLIBDecompressor.hpp"
//Local
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Mathematics.hpp>
#include <Std++/Streams/BufferInputStream.hpp>
#include <Std++/Constants.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

class ZLIBDecompressor::ZLIBWrapperInputStream : public InputStream
{
public:
	//Constructor
	inline ZLIBWrapperInputStream(InputStream& baseInputStream) : baseInputStream(baseInputStream), nBytesInBuffer(0)
	{
	}

	//Methods
	uint32 GetBytesAvailable() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return 0;
	}

	uint32 GetChecksum() const
	{
		ASSERT(this->IsAtEnd() && (this->nBytesInBuffer == 4), u8"MUST HAVE THE TRAILER!");

		BufferInputStream bufferInputStream(this->buffer, sizeof(this->buffer));
		DataReader dataReader(true, bufferInputStream);
		return dataReader.ReadUInt32();
	}

	bool IsAtEnd() const override
	{
		return this->baseInputStream.IsAtEnd();
	}

	uint32 ReadBytes(void *destination, uint32 count) override
	{
		uint32 bytesReadTotal = 0;

		if(this->baseInputStream.IsAtEnd())
			return 0;

		byte* dest = static_cast<byte *>(destination);
		//consume from buffer
		if(this->nBytesInBuffer > 0)
		{
			uint32 fromBuffer = Math::Min(count, uint32(this->nBytesInBuffer));
			MemCopy(dest, this->buffer + sizeof(this->buffer) - this->nBytesInBuffer, fromBuffer);


			dest += fromBuffer;
			bytesReadTotal += fromBuffer;
			count -= fromBuffer;
			this->nBytesInBuffer -= fromBuffer;

			if(this->nBytesInBuffer != 0)
				this->ShiftBuffer(static_cast<uint8>(fromBuffer));
		}

		//read user data from base stream
		uint32 readBytes = this->baseInputStream.ReadBytes(dest, count);
		bytesReadTotal += readBytes;

		//get trailer data
		if(readBytes < count)
		{
			NOT_IMPLEMENTED_ERROR; //TODO: steal the last 4 bytes from input
		}
		else
		{
			//read in trailer extra
			uint32 nBytesToRead = sizeof(this->buffer) - this->nBytesInBuffer;
			readBytes = this->baseInputStream.ReadBytes(this->buffer + this->nBytesInBuffer, nBytesToRead);
			if(readBytes < nBytesToRead)
			{
				NOT_IMPLEMENTED_ERROR; //TODO: steal the last missing bytes from input
			}
			else
			{
				this->nBytesInBuffer += readBytes;
				if(this->IsAtEnd())
				{
					ASSERT(this->nBytesInBuffer == 4, u8"MUST HAVE THE TRAILER!");
				}
			}
		}

		return bytesReadTotal;
	}

	uint32 Skip(uint32 nBytes) override
	{
		byte buffer[c_io_blockSize];

		uint32 nBytesSkipped = 0;
		while(nBytes)
		{
			uint32 left = Math::Min(nBytes, uint32(sizeof(buffer)));
			uint32 nBytesRead = this->ReadBytes(buffer, left);

			nBytesSkipped += nBytesRead;
			nBytes -= nBytesRead;
		}

		return nBytesSkipped;
	}

private:
	//Members
	InputStream& baseInputStream;
	byte buffer[4];
	uint8 nBytesInBuffer;

	//Inline
	inline void ShiftBuffer(uint8 shiftAmount)
	{
		for(uint8 i = 0; i < this->nBytesInBuffer; i++)
			this->buffer[i] = this->buffer[i + shiftAmount];
	}
};

//Constructor
ZLIBDecompressor::ZLIBDecompressor(InputStream &inputStream, bool verify) : Decompressor(inputStream)
{
	//section 2.2
	DataReader dataReader(true, inputStream);

	uint8 compressionMethodAndFlags = dataReader.ReadByte();

	CompressionAlgorithm compressionAlgorithm;
	switch(compressionMethodAndFlags & 0xF)
	{
		case 8:
			compressionAlgorithm = CompressionAlgorithm::DEFLATE;
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	uint8 flags = dataReader.ReadByte();
	if(verify)
	{
		uint16 checker = (uint16(compressionMethodAndFlags) << 8) | flags;
		ASSERT((checker % 31) == 0, u8"Invalid zlib header");

		this->verifier = ChecksumFunction::CreateInstance(ChecksumAlgorithm::ADLER32);
	}

	if(flags & 0x20)
	{
		NOT_IMPLEMENTED_ERROR; //TODO: present dictionary....
	}

	this->wrapperInputStream = new ZLIBWrapperInputStream(inputStream);
	this->realDecompressor = Decompressor::Create(compressionAlgorithm, *this->wrapperInputStream, verify);
}

//Public methods
uint32 _stdxx_::ZLIBDecompressor::GetBytesAvailable() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

bool _stdxx_::ZLIBDecompressor::IsAtEnd() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return false;
}

uint32 ZLIBDecompressor::ReadBytes(void *destination, uint32 count)
{
	uint8* dest = static_cast<uint8 *>(destination);
	while(count)
	{
		uint32 nBytesRead = this->realDecompressor->ReadBytes(dest, count);
		if(!this->verifier.IsNull())
			this->verifier->Update(dest, nBytesRead);
		dest += nBytesRead;
		count -= nBytesRead;

		if(this->realDecompressor->IsAtEnd())
		{
			if(!this->verifier.IsNull())
			{
				this->verifier->Finish();
				uint32 computed = this->verifier->GetChecksum();
				uint32 read = this->wrapperInputStream->GetChecksum();

				ASSERT(read == computed, u8"Checksum mismatch");
			}
			break;
		}
	}

	return dest - (uint8*)destination;
}

uint32 _stdxx_::ZLIBDecompressor::Skip(uint32 nBytes)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}
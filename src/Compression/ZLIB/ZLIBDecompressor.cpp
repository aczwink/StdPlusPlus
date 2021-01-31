/*
 * Copyright (c) 2019-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/LimitedEndInputStream.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

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

	this->wrapperInputStream = new LimitedEndInputStream(inputStream, 4);
	this->realDecompressor = Decompressor::Create(compressionAlgorithm, *this->wrapperInputStream);
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
				uint32 read = this->ReadChecksum();

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

//Private methods
uint32 ZLIBDecompressor::ReadChecksum()
{
	ASSERT(this->wrapperInputStream->IsAtEnd(), u8"The checksum is at the end!");

	BufferInputStream bufferInputStream(this->wrapperInputStream->EndBytes(), 4);
	DataReader dataReader(true, bufferInputStream);
	return dataReader.ReadUInt32();
}

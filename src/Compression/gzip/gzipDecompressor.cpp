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
#include "gzipDecompressor.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
//Local
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Streams/TextCodec.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>
#include <Std++/Streams/SeekableInputStream.hpp>

//Constructor
gzipDecompressor::gzipDecompressor(InputStream &inputStream, bool verify) : Decompressor(inputStream), memberHeaderRead(false)
{
	this->reachedEnd = false;
	this->inflater = Decompressor::Create(CompressionAlgorithm::DEFLATE, inputStream);
	if(verify)
		this->crc = ChecksumFunction::CreateInstance(ChecksumAlgorithm::CRC32);
}

//Public methods
uint32 _stdxx_::gzipDecompressor::GetBytesAvailable() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

bool gzipDecompressor::IsAtEnd() const
{
	return this->inflater->IsAtEnd();
}

uint32 gzipDecompressor::ReadBytes(void *destination, uint32 count)
{
	if(this->reachedEnd)
		return 0;
	this->EnsureMemberHeaderRead();

	uint8* dest = static_cast<uint8 *>(destination);
	while(count)
	{
		uint32 nBytesRead = this->inflater->ReadBytes(dest, count);
		if(!this->crc.IsNull())
			this->crc->Update(dest, nBytesRead);

		dest += nBytesRead;
		count -= nBytesRead;

		if(this->inflater->IsAtEnd())
		{
			/*
			 * TODO: this class can only read the first member stream while in theory there could be many.
			 * TODO: The problem is that the compressed size is not known and therefore the whole file would have to be uncompressed in order to know whether there is only one embedded stream or many
			 * TODO: in addition, the deflater will read past the end of the deflate input and the rest stays in its input buffer, while it should remain in this->inputStream
			 * TODO: the zlib stream (ZLIBDecompressor class) has the same problem
			 */
			this->reachedEnd = true;
			if(!this->crc.IsNull())
			{
				ASSERT(IS_INSTANCE_OF(&this->inputStream, SeekableInputStream), u8"TODO: do this correctly");
				SeekableInputStream &seekableInputStream = (SeekableInputStream &) this->inputStream;
				seekableInputStream.Rewind(8);
				DataReader dataReader(false, seekableInputStream);
				uint32 expectedCrc = dataReader.ReadUInt32();
				this->crc->Finish();
				ASSERT(expectedCrc == this->crc->GetChecksum(), u8"REPORT THIS PLEASE");
			}
			break;
		}
	}

	return dest - static_cast<uint8 *>(destination);
}

//Private methods
void gzipDecompressor::EnsureMemberHeaderRead()
{
	if(this->memberHeaderRead)
		return;

	DataReader dataReader(false, this->inputStream);

	uint16 signature = dataReader.ReadUInt16();
	ASSERT(signature == 0x8b1f, u8"REPORT THIS PLEASE!");
	uint8 compressionMethod = dataReader.ReadByte();
	ASSERT(compressionMethod == 8, u8"REPORT THIS PLEASE");
	uint8 flags = dataReader.ReadByte();
	ASSERT(flags == 8, u8"REPORT THIS PLEASE!");
	uint32 modificationTime = dataReader.ReadUInt32();

	dataReader.Skip(1); //extra flags
	dataReader.Skip(1); //operating system

	if(flags & 8u)
	{
		TextReader textReader(this->inputStream, TextCodecType::Latin1);
		textReader.ReadZeroTerminatedString();
	}

	this->memberHeaderRead = true;
}

uint32 gzipDecompressor::Skip(uint32 nBytes)
{
	return ReadOnlyInputStream::Skip(nBytes);
}

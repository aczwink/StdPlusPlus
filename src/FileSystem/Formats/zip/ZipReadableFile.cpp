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
/*//Class header
#include "ZipReadableFile.hpp"
//Local
#include <Std++/Streams/ChainedInputStream.hpp>
#include <Std++/FileSystem/EmbeddedFileInputStream.hpp>
#include <Std++/Streams/BufferedInputStream.hpp>
#include <Std++/Streams/CheckedInputStream.hpp>
#include <Std++/Compression/Decompressor.hpp>
#include "AEDecrypter.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Constructor
ZipReadableFile::ZipReadableFile(const CentralDirectoryRecord &centralDirectoryRecord, ZipFileSystem &fileSystem)
	: fileSystem(fileSystem), fileHeader(centralDirectoryRecord)
{
	this->fileDataOffset = fileSystem.InputStream().QueryCurrentOffset();
	LocalFileHeader localFileHeader(fileSystem.InputStream());
	this->fileDataOffset += localFileHeader.HeaderSize();
}

//Public methods
UniquePointer<InputStream> ZipReadableFile::OpenForReading(bool verify) const
{
	ChainedInputStream* chain = new ChainedInputStream(
			new EmbeddedFileInputStream(this->fileDataOffset, this->fileHeader.compressedSize, this->fileSystem.InputStream(), this->fileSystem.StreamLock()));

	if(this->fileHeader.compressionMethod == 99)
	{
		chain->Add(new BufferedInputStream(chain->GetEnd())); //add a buffer for performance
		chain->Add(new AEDecrypter(this->fileHeader.compressedSize, this->fileHeader.compressionMethodData.AE.strength, this->fileSystem.Password(), chain->GetEnd(), verify));
		if(this->fileHeader.compressionMethodData.AE.actualCompressionMethod)
			chain->Add(Decompressor::Create(this->MapCompressionMethod(this->fileHeader.compressionMethodData.AE.actualCompressionMethod), chain->GetEnd()));

		if(this->fileHeader.crc32 == 0)
			verify = false;
	}
	else if(this->fileHeader.compressionMethod != 0)
	{
		chain->Add(new BufferedInputStream(chain->GetEnd())); //add a buffer for performance
		chain->Add(Decompressor::Create(this->MapCompressionMethod(this->fileHeader.compressionMethod), chain->GetEnd()));
	}

	if(verify)
    {
	    chain->Add(new CheckedInputStream(chain->GetEnd(), ChecksumAlgorithm::CRC32, this->fileHeader.crc32));
    }

	return chain;
}

//Private methods
CompressionAlgorithm ZipReadableFile::MapCompressionMethod(uint16 compressionMethod) const
{
	switch(compressionMethod)
	{
		case 8:
			return CompressionAlgorithm::DEFLATE;
	}

	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
*/
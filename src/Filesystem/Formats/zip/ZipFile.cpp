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
#include "ZipFile.hpp"
//Local
#include <Std++/Streams/BufferedInputStream.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Filesystem/EmbeddedFileInputStream.hpp>
#include <Std++/Streams/ChainedInputStream.hpp>
#include <Std++/Compression/Decompressor.hpp>
#include <Std++/Streams/CheckedInputStream.hpp>
#include "Zip.hpp"
#include "ZipFileSystem.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Constructor
ZipFile::ZipFile(const CentralDirectoryRecord& centralDirectoryRecord, ZipFileSystem& fileSystem)
	: fileSystem(fileSystem), fileHeader(centralDirectoryRecord)
{
    this->fileDataOffset = fileSystem.InputStream().GetCurrentOffset();
    LocalFileHeader localFileHeader(fileSystem.InputStream());
    this->fileDataOffset += localFileHeader.HeaderSize();
}

//Public methods
uint64 ZipFile::GetSize() const
{
	return this->fileHeader.uncompressedSize;
}

UniquePointer<InputStream> ZipFile::OpenForReading(bool verify) const
{
	ChainedInputStream* chain = new ChainedInputStream(
			new EmbeddedFileInputStream(this->fileDataOffset, this->fileHeader.compressedSize, this->fileSystem.InputStream(), this->fileSystem.StreamLock()));

	if(this->fileHeader.compressionMethod != 0)
	{
		chain->Add(new BufferedInputStream(chain->GetEnd())); //add a buffer for performance
		chain->Add(Decompressor::Create(this->MapCompressionMethod(), chain->GetEnd(), verify));
	}

	if(verify)
    {
	    chain->Add(new CheckedInputStream(chain->GetEnd(), ChecksumAlgorithm::CRC32, this->fileHeader.crc32));
    }

	return chain;
}

StdXX::UniquePointer<StdXX::OutputStream> _stdxx_::ZipFile::OpenForWriting() {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::UniquePointer<StdXX::OutputStream>();
}

StdXX::FileSystemNodeInfo _stdxx_::ZipFile::QueryInfo() const {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::FileSystemNodeInfo();
}

//Private methods
CompressionAlgorithm ZipFile::MapCompressionMethod() const
{
	switch(this->fileHeader.compressionMethod)
	{
		case 8:
			return CompressionAlgorithm::DEFLATE;
	}

	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

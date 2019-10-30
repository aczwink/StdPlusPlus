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
#include "Zip.hpp"
#include "ZipFileSystem.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Constructor
ZipFile::ZipFile(uint32 compressedSize, uint32 crc32, ZipFileSystem& fileSystem)
	: fileSystem(fileSystem), compressedSize(compressedSize)
{
	this->ReadFileMetadata(fileSystem.InputStream());
}

//Public methods
uint64 _stdxx_::ZipFile::GetSize() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

UniquePointer<InputStream> ZipFile::OpenForReading(bool verify) const
{
	ChainedInputStream* chain = new ChainedInputStream(
			new EmbeddedFileInputStream(this->fileDataOffset, this->compressedSize, this->fileSystem.InputStream(), this->fileSystem.StreamLock()));

	if(this->compressionMethod != 0)
	{
		chain->Add(new BufferedInputStream(chain->GetEnd())); //add a buffer for performance
		chain->Add(Decompressor::Create(this->MapCompressionMethod(), chain->GetEnd(), verify));
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
void ZipFile::ReadFileMetadata(SeekableInputStream &inputStream)
{
	BufferedInputStream bufferedInputStream(inputStream);
	DataReader dataReader(false, bufferedInputStream);

	ASSERT(dataReader.ReadUInt32() == zipLocalFileHeaderSignature, u8"REPORT THIS PLEASE!");
	bufferedInputStream.Skip(2);
	bufferedInputStream.Skip(2);
	this->compressionMethod = dataReader.ReadUInt16();
	bufferedInputStream.Skip(2);
	bufferedInputStream.Skip(2);
	bufferedInputStream.Skip(8); //compressed size and crc32 can be zero here
	bufferedInputStream.Skip(4);
	uint16 fileNameSize = dataReader.ReadUInt16();
	uint16 extraFieldSize = dataReader.ReadUInt16();

	dataReader.Skip(fileNameSize);
	dataReader.Skip(extraFieldSize);

	this->fileDataOffset = inputStream.GetCurrentOffset() - bufferedInputStream.GetBytesAvailable();
}

CompressionAlgorithm ZipFile::MapCompressionMethod() const
{
	switch(this->compressionMethod)
	{
		case 8:
			return CompressionAlgorithm::DEFLATE;
	}

	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

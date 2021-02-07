/*
* Copyright (c) 2018-2021 Amir Czwink (amir130@hotmail.de)
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
/*
//Class header
#include "7zip_FileSystem.hpp"
//Local
#include <Std++/Compression/Decompressor.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>
#include <Std++/Streams/LimitedInputStream.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::FileSystem;

//Public methods
void SevenZip_FileSystem::Flush()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

AutoPointer<const Node> SevenZip_FileSystem::GetNode(const Path &path) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::AutoPointer<const StdXX::FileSystem::Node>();
}

//Private methods
CompressionAlgorithm SevenZip_FileSystem::MapCodecId(byte(&codecId)[16], uint8 codecIdSize) const
{
	ASSERT(codecIdSize >= 1, u8"Report this please!");

	switch (codecId[0])
	{
	case 3: //7z
	{
		switch (codecId[1])
		{
		case 1:
		{
			ASSERT(codecIdSize == 3, u8"Report this please!");
			ASSERT(codecId[2] == 1, u8"Report this please!");
			
			return CompressionAlgorithm::LZMA;
		}
		break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	}
	break;
	case 0x21: //LZMA2
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		//return CompressionAlgorithm::LZMA2;
	default:
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
}

uint8 SevenZip_FileSystem::ReadArchiveProperties()
{
	NOT_IMPLEMENTED_ERROR; //TODO: this method was never tested
	/*
	DataReader reader(false, *this->containerInputStream); //7z is little endian

	byte id = reader.ReadByte();
	ASSERT(id == (byte)PropertyId::kArchiveProperties, u8"Report this please!");
	while (true)
	{
		byte propertyType = reader.ReadByte();
		if (propertyType == 0)
			break;
		uint64 propertySize = this->ReadVariableLengthUInt();
		reader.Skip(propertySize); //property data
	}

	NOT_IMPLEMENTED_ERROR; //TODO: implement me*/
	/*return -1;
}

void SevenZip_FileSystem::ReadCodersInfo(CodersInfo& codersInfo, InputStream& inputStream)
{
	DataReader reader(false, inputStream); //7z is little endian

	byte id = reader.ReadByte();
	ASSERT(id == (byte)PropertyId::kFolder, u8"Report this please!");

	uint64 nFolders = this->ReadVariableLengthUInt(inputStream);
	codersInfo.folderInfos.Resize(nFolders);
	bool external = reader.ReadByte() != 0;
	if (external)
	{
		uint64 dataStreamIndex = this->ReadVariableLengthUInt(inputStream);

		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
	else
	{
		for (uint64 i = 0; i < nFolders; i++)
			this->ReadFolder(codersInfo.folderInfos[i].folder, inputStream);
	}

	id = reader.ReadByte();
	ASSERT(id == (byte)PropertyId::kCodersUnPackSize, u8"Report this please!");
	for (uint64 i = 0; i < nFolders; i++)
	{
		codersInfo.folderInfos[i].uncompressedSize = this->ReadVariableLengthUInt(inputStream);
	}

	id = reader.ReadByte();
	if (id == (byte)PropertyId::kCRC)
	{
		this->ReadDigests(nFolders, inputStream);
		id = reader.ReadByte();
	}

	ASSERT(id == (byte)PropertyId::kEnd, u8"Report this please!");
}

void SevenZip_FileSystem::ReadDigests(uint64 nStreams, InputStream& inputStream)
{
	DataReader reader(false, inputStream); //7z is little endian

	bool allAreDefined = reader.ReadByte() != 0;
	uint64 nDefinedDigests;
	if (!allAreDefined)
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
	else
	{
		nDefinedDigests = nStreams;
	}

	for (uint64 i = 0; i < nDefinedDigests; i++)
	{
		reader.ReadUInt32(); //crc
	}
}

void SevenZip_FileSystem::ReadFilesInfo(InputStream& inputStream)
{
	DataReader reader(false, inputStream); //7z is little endian
	TextReader textReader(inputStream, TextCodecType::UTF16_LE);

	uint64 nFiles = this->ReadVariableLengthUInt(inputStream);

	while (true)
	{
		PropertyId id = (PropertyId)reader.ReadByte();
		if (id == PropertyId::kEnd)
			break;
		uint64 size = this->ReadVariableLengthUInt(inputStream);
		
		switch (id)
		{
		case PropertyId::kName:
		{
			bool external = reader.ReadByte() != 0;
			size--;
			if (external)
			{
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			}

			if (size)
			{
				for (uint64 i = 0; i < nFiles; i++)
				{
					textReader.ReadZeroTerminatedString();
				}
			}
		}
		break;
		case PropertyId::kMTime:
		{
			bool allAreDefined = reader.ReadByte() != 0;
			uint64 nDefined;
			if (!allAreDefined)
			{
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			}
			else
			{
				nDefined = nFiles;
			}
			bool external = reader.ReadByte() != 0;
			if (external)
			{
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			}

			for (uint64 i = 0; i < nDefined; i++)
			{
				reader.ReadUInt64(); //time
			}
		}
		break;
		case PropertyId::kDummy: //???
		case PropertyId::kWinAttributes: //even more ???
			reader.Skip(size);
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	}
}

void SevenZip_FileSystem::ReadFolder(Folder& folder, InputStream& inputStream)
{
	DataReader reader(false, inputStream); //7z is little endian

	uint64 nCoders = this->ReadVariableLengthUInt(inputStream);
	folder.coders.Resize(nCoders);
	for (uint64 i = 0; i < nCoders; i++)
	{
		auto& coder = folder.coders[i];

		byte flags = reader.ReadByte();
		uint8 codecIdSize = flags & 0xF;
		byte codecId[16];
		reader.ReadBytes(codecId, codecIdSize);
		if ((codecIdSize == 1) && (codecId[0] == 0))
			coder.compressed = false;
		else
		{
			coder.compressed = true;
			coder.method = this->MapCodecId(codecId, codecIdSize);
		}
		if (flags & 0x10)
		{
			//complex
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}

		if (flags & 0x20)
		{
			uint64 propertiesSize = this->ReadVariableLengthUInt(inputStream);
			coder.properties = FixedArray<byte>(propertiesSize);
			reader.ReadBytes((void*)&(*coder.properties)[0], propertiesSize);
		}
	}
}

void SevenZip_FileSystem::ReadHeader(uint64 offset, uint64 size)
{
	this->containerInputStream.SeekTo(this->baseOffset + offset);

	DataReader reader(false, this->containerInputStream); //7z is little endian

	PropertyId id = (PropertyId)reader.ReadByte();
	switch (id)
	{
	case PropertyId::kEncodedHeader:
	{
		StreamsInfo streamsInfo;
		this->ReadStreamsInfo(streamsInfo, this->containerInputStream);

		this->containerInputStream.SeekTo(this->baseOffset + streamsInfo.packInfo->offset);
		ASSERT(streamsInfo.packInfo->packedStreams.GetNumberOfElements() == 1, u8"Report this please!");
		ASSERT(streamsInfo.codersInfo->folderInfos.GetNumberOfElements() == 1, u8"Report this please!");
		ASSERT(streamsInfo.codersInfo->folderInfos[0].folder.coders.GetNumberOfElements() == 1, u8"Report this please!");
		
		LimitedInputStream limiter(this->containerInputStream, streamsInfo.packInfo->packedStreams[0].compressedSize);
		const auto& coder = streamsInfo.codersInfo->folderInfos[0].folder.coders[0];

		UniquePointer<Decompressor> decompressor;
		NOT_IMPLEMENTED_ERROR; //TODO: reimplement me
		//UniquePointer<Decompressor> decompressor = Decompressor::CreateRaw(coder.method, limiter, &(*coder.properties)[0], coder.properties->GetNumberOfElements(), streamsInfo.codersInfo->folderInfos[0].uncompressedSize);

		decompressor->ReadBytes(&id, 1);
		ASSERT(id == PropertyId::kHeader, u8"Report this please!");

		this->ReadHeaderData(*decompressor);
	}
	break;
	case PropertyId::kHeader:
		this->ReadHeaderData(this->containerInputStream);
	default:
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
}

void SevenZip_FileSystem::ReadHeaderData(InputStream& inputStream)
{
	DataReader reader(false, inputStream); //7z is little endian

	PropertyId id = (PropertyId)reader.ReadByte();
	if (id == PropertyId::kMainStreamsInfo)
	{
		StreamsInfo streamsInfo;
		this->ReadStreamsInfo(streamsInfo, inputStream);
		id = (PropertyId)reader.ReadByte();
	}

	if (id == PropertyId::kFilesInfo)
	{
		this->ReadFilesInfo(inputStream);
		id = (PropertyId)reader.ReadByte();
	}

	ASSERT(id == PropertyId::kEnd, u8"Report this please!");
}

void SevenZip_FileSystem::ReadFileSystemHeader()
{
	DataReader reader(false, this->containerInputStream); //7z is little endian
	
	reader.Skip(6); //the signature
	byte versionMajor = reader.ReadByte();
	byte versionMinor = reader.ReadByte();

	reader.Skip(4); //startHeaderCRC
	uint64 nextHeaderOffset = reader.ReadUInt64();
	uint64 nextHeaderSize = reader.ReadUInt64();
	reader.Skip(4); //nextHeaderCRC

	this->baseOffset = this->containerInputStream.GetCurrentOffset();
	this->ReadHeader(nextHeaderOffset, nextHeaderSize);
}

void SevenZip_FileSystem::ReadPackInfo(PackInfo& packInfo, InputStream& inputStream)
{
	packInfo.offset = this->ReadVariableLengthUInt(inputStream);
	uint64 numPackStreams = this->ReadVariableLengthUInt(inputStream);

	DataReader reader(false, inputStream); //7z is little endian

	packInfo.packedStreams.Resize(numPackStreams);

	byte id = reader.ReadByte();
	if (id == (byte)PropertyId::kSize)
	{
		for(uint64 i = 0; i < numPackStreams; i++)
			packInfo.packedStreams[i].compressedSize = this->ReadVariableLengthUInt(inputStream);
		id = reader.ReadByte();
	}

	if (id == (byte)PropertyId::kCRC)
	{
		for (uint64 i = 0; i < numPackStreams; i++)
			packInfo.packedStreams[i].crc32 = reader.ReadUInt32();
		id = reader.ReadByte();
	}

	ASSERT(id == (byte)PropertyId::kEnd, u8"Report this please!");
}

void SevenZip_FileSystem::ReadStreamsInfo(StreamsInfo& streamsInfo, InputStream& inputStream)
{
	DataReader reader(false, inputStream); //7z is little endian

	byte id = reader.ReadByte();
	if (id == (byte)PropertyId::kPackInfo)
	{
		PackInfo packInfo;
		this->ReadPackInfo(packInfo, inputStream);
		streamsInfo.packInfo = StdXX::Move(packInfo);
		id = reader.ReadByte();
	}

	if (id == (byte)PropertyId::kUnPackInfo)
	{
		CodersInfo codersInfo;
		this->ReadCodersInfo(codersInfo, inputStream);
		streamsInfo.codersInfo = StdXX::Move(codersInfo);
		id = reader.ReadByte();
	}

	if (id == (byte)PropertyId::kSubStreamsInfo)
	{
		this->ReadSubStreamsInfo(streamsInfo, inputStream);
		id = reader.ReadByte();
	}

	ASSERT(id == (byte)PropertyId::kEnd, u8"Report this please!");
}

void SevenZip_FileSystem::ReadSubStreamsInfo(const StreamsInfo& streamsInfo, InputStream& inputStream)
{
	DataReader reader(false, inputStream); //7z is little endian
	
	byte id = reader.ReadByte();
	if (id == (byte)PropertyId::kNumUnPackStream)
	{
		for(uint64 i = 0; i < streamsInfo.codersInfo->folderInfos.GetNumberOfElements(); i++)
			this->ReadVariableLengthUInt(inputStream);
		id = reader.ReadByte();
	}

	if (id == (byte)PropertyId::kSize)
	{
		NOT_IMPLEMENTED_ERROR; //TODO: this doesnt work correctly
		for (uint64 i = 0; i < streamsInfo.codersInfo->folderInfos.GetNumberOfElements(); i++)
		{
			for (uint64 j = 0; j < 1; j++) //TODO: implement this for nOutputStreams
			{
				this->ReadVariableLengthUInt(inputStream);
			}
		}

		id = reader.ReadByte();
	}

	if (id == (byte)PropertyId::kCRC)
	{
		uint64 nUnknownCRC = 1; //TODO: implement me
		this->ReadDigests(nUnknownCRC, inputStream);
		id = reader.ReadByte();
	}

	ASSERT(id == (byte)PropertyId::kEnd, u8"Report this please!");
}

uint64 SevenZip_FileSystem::ReadVariableLengthUInt(InputStream& inputStream)
{
	DataReader reader(false, inputStream); //7z is little endian
	
	uint8 first = reader.ReadByte();
	uint8 mask = 0x80;
	uint8 length = 1;

	//find length
	while (mask & first)
	{
		mask >>= 1;
		length++;
	}
	
	const uint8 nExtraBytes = length - 1;
	uint64 result = 0;
	for(uint8 i = 0; i < nExtraBytes; i++)
		result |= uint64(reader.ReadByte()) << i * 8;

	mask = (1 << (8 - length)) - 1;
	result |= (first & mask) << nExtraBytes * 8;
	
	return result;
}*/
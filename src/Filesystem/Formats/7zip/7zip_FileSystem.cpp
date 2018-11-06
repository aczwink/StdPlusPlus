/*
* Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include "7zip_FileSystem.hpp"
//Local
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Public methods
void SevenZip_FileSystem::Flush()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

bool SevenZip_FileSystem::IsDirectory(const Path & path) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return false;
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
	default:
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
}

uint8 SevenZip_FileSystem::ReadArchiveProperties()
{
	NOT_IMPLEMENTED_ERROR; //TODO: this method was never tested

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

	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return -1;
}

void SevenZip_FileSystem::ReadCodersInfo(CodersInfo& codersInfo)
{
	DataReader reader(false, *this->containerInputStream); //7z is little endian

	byte id = reader.ReadByte();
	ASSERT(id == (byte)PropertyId::kFolder, u8"Report this please!");

	uint64 nFolders = this->ReadVariableLengthUInt();
	codersInfo.folderInfos.Resize(nFolders);
	bool external = reader.ReadByte() != 0;
	if (external)
	{
		uint64 dataStreamIndex = this->ReadVariableLengthUInt();

		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
	else
	{
		for (uint64 i = 0; i < nFolders; i++)
			this->ReadFolder(codersInfo.folderInfos[i].folder);
	}

	id = reader.ReadByte();
	ASSERT(id == (byte)PropertyId::kCodersUnPackSize, u8"Report this please!");
	for (uint64 i = 0; i < nFolders; i++)
	{
		codersInfo.folderInfos[i].uncompressedSize = this->ReadVariableLengthUInt();
	}

	id = reader.ReadByte();
	if (id == (byte)PropertyId::kCRC)
	{
		this->ReadDigests(nFolders);
		id = reader.ReadByte();
	}

	ASSERT(id == (byte)PropertyId::kEnd, u8"Report this please!");
}

void SevenZip_FileSystem::ReadDigests(uint64 nStreams)
{
	DataReader reader(false, *this->containerInputStream); //7z is little endian

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

void SevenZip_FileSystem::ReadFilesInfo()
{
	DataReader reader(false, *this->containerInputStream); //7z is little endian
	TextReader textReader(*this->containerInputStream, TextCodecType::UTF16_LE);

	uint64 nFiles = this->ReadVariableLengthUInt();

	while (true)
	{
		PropertyId id = (PropertyId)reader.ReadByte();
		if (id == PropertyId::kEnd)
			break;
		uint64 size = this->ReadVariableLengthUInt();
		
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

void SevenZip_FileSystem::ReadFolder(Folder& folder)
{
	DataReader reader(false, *this->containerInputStream); //7z is little endian

	uint64 nCoders = this->ReadVariableLengthUInt();
	folder.coders.Resize(nCoders);
	for (uint64 i = 0; i < nCoders; i++)
	{
		byte flags = reader.ReadByte();
		uint8 codecIdSize = flags & 0xF;
		byte codecId[16];
		reader.ReadBytes(codecId, codecIdSize);
		folder.coders[i] = this->MapCodecId(codecId, codecIdSize);
		if (flags & 0x10)
		{
			//complex
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}

		if (flags & 0x20)
		{
			uint64 propertiesSize = this->ReadVariableLengthUInt();
			reader.Skip(propertiesSize);
		}
	}
}

void SevenZip_FileSystem::ReadHeader(uint64 offset, uint64 size)
{
	this->containerInputStream->SetCurrentOffset(this->baseOffset + offset);

	DataReader reader(false, *this->containerInputStream); //7z is little endian

	PropertyId id = (PropertyId)reader.ReadByte();
	switch (id)
	{
	case PropertyId::kEncodedHeader:
	{
		this->ReadStreamsInfo();
		uint64 _cur = this->containerInputStream->GetRemainingBytes();
		_cur++;
		_cur--;
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
	break;
	case PropertyId::kHeader:
		id = (PropertyId)reader.ReadByte();
		break;
	default:
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	if (id == PropertyId::kMainStreamsInfo)
	{
		this->ReadStreamsInfo();
		id = (PropertyId)reader.ReadByte();
	}

	if (id == PropertyId::kFilesInfo)
	{
		this->ReadFilesInfo();
		id = (PropertyId)reader.ReadByte();
	}

	ASSERT(id == PropertyId::kEnd, u8"Report this please!");
}

void SevenZip_FileSystem::ReadFileSystemHeader()
{
	DataReader reader(false, *this->containerInputStream); //7z is little endian
	
	reader.Skip(6); //the signature
	byte versionMajor = reader.ReadByte();
	byte versionMinor = reader.ReadByte();

	reader.Skip(4); //startHeaderCRC
	uint64 nextHeaderOffset = reader.ReadUInt64();
	uint64 nextHeaderSize = reader.ReadUInt64();
	reader.Skip(4); //nextHeaderCRC

	this->baseOffset = this->containerInputStream->GetCurrentOffset();
	this->ReadHeader(nextHeaderOffset, nextHeaderSize);
}

void SevenZip_FileSystem::ReadPackInfo(PackInfo& packInfo)
{
	packInfo.offset = this->ReadVariableLengthUInt();
	uint64 numPackStreams = this->ReadVariableLengthUInt();

	DataReader reader(false, *this->containerInputStream); //7z is little endian

	packInfo.packedStreams.Resize(numPackStreams);

	byte id = reader.ReadByte();
	if (id == (byte)PropertyId::kSize)
	{
		for(uint64 i = 0; i < numPackStreams; i++)
			packInfo.packedStreams[i].compressedSize = this->ReadVariableLengthUInt();
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

void SevenZip_FileSystem::ReadStreamsInfo()
{
	DataReader reader(false, *this->containerInputStream); //7z is little endian

	byte id = reader.ReadByte();
	if (id == (byte)PropertyId::kPackInfo)
	{
		PackInfo packInfo;
		this->ReadPackInfo(packInfo);
		id = reader.ReadByte();
	}

	if (id == (byte)PropertyId::kUnPackInfo)
	{
		CodersInfo codersInfo;
		this->ReadCodersInfo(codersInfo);
		id = reader.ReadByte();
	}

	if (id == (byte)PropertyId::kSubStreamsInfo)
	{
		this->ReadSubStreamsInfo();
		id = reader.ReadByte();
	}

	ASSERT(id == (byte)PropertyId::kEnd, u8"Report this please!");
}

void SevenZip_FileSystem::ReadSubStreamsInfo()
{
	DataReader reader(false, *this->containerInputStream); //7z is little endian
	
	byte id = reader.ReadByte();

	if (id == (byte)PropertyId::kCRC)
	{
		uint64 nUnknownCRC = 1; //TODO: implement me
		this->ReadDigests(nUnknownCRC);
		id = reader.ReadByte();
	}

	ASSERT(id == (byte)PropertyId::kEnd, u8"Report this please!");
}

uint64 SevenZip_FileSystem::ReadVariableLengthUInt()
{
	DataReader reader(false, *this->containerInputStream); //7z is little endian
	
	uint8 first = reader.ReadByte();
	uint8 mask = 0x80;
	uint8 length = 1;

	//find length
	while (mask & first)
	{
		mask >>= 1;
		length++;
	}
	
	mask = (1 << (8 - length)) - 1;
	uint64 result = first & mask;

	for(uint8 i = 1; i < length; i++)
		result = (result << 8) | reader.ReadByte();
	
	return result;
}
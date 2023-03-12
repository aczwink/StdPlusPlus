/*
 * Copyright (c) 2023 Amir Czwink (amir130@hotmail.de)
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
//Main header
#include "ID3.hpp"
//Local
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Multimedia/CodingFormatId.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>

//Local functions
//ID3V2
static String ReadID3V2String(uint8 textEncoding, uint32 dataSize, SeekableInputStream& inputStream)
{
	String buffer;

	DataReader leReader(false, inputStream);
	DataReader beReader(true, inputStream);

	switch(textEncoding)
	{
		case 0: //ISO-8859-1
		{
			while(dataSize--)
			{
				buffer += uint16((char)leReader.ReadByte());
			}
		}
			break;
		case 1: //Unicode 16bit, UCS-2
		{
			uint16 bom, c;

			bom = leReader.ReadUInt16();
			dataSize -= 2;

			while(dataSize > 0)
			{
				if(bom == 0xFFFE)
					c = beReader.ReadUInt16();
				else
					c = leReader.ReadUInt16();

				buffer += (uint16)c;
				dataSize -= 2;
			}
		}
			break;
		case 3: //UTF-8
		{
			uint32 codePoint;
			uint64 offset;
			TextReader reader(inputStream, TextCodecType::UTF8);

			while(dataSize)
			{
				offset = inputStream.QueryCurrentOffset();
				codePoint = reader.ReadCodePoint();
				offset = inputStream.QueryCurrentOffset() - offset;

				dataSize -= offset;
				buffer += codePoint;
			}
		}
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	return buffer;
}

static String ReadID3V2String(uint32 dataSize, SeekableInputStream& inputStream)
{
	DataReader dataReader(false, inputStream);

	return ReadID3V2String(dataReader.ReadByte(), dataSize - 1, inputStream);
}

static uint32 ReadID3V2Frame(SeekableInputStream& inputStream, SMetaInfo &refMetaInfo)
{
	uint16 flags;
	uint32 frameId, dataSize;

	DataReader leReader(false, inputStream);
	DataReader beReader(true, inputStream);

	frameId = leReader.ReadUInt32();
	if(frameId == 0)
	{
		//padding
		inputStream.Rewind(4);
		return 0;
	}

	dataSize = beReader.ReadUInt32();
	flags = leReader.ReadUInt16();

	switch(frameId)
	{
		case FOURCC("TALB"):
		{
			refMetaInfo.album = ReadID3V2String(dataSize, inputStream);
		}
			break;
		case FOURCC("TCOM"):
		{
			refMetaInfo.composer = ReadID3V2String(dataSize, inputStream);
		}
			break;
		case FOURCC("TIT2"):
		{
			refMetaInfo.title = ReadID3V2String(dataSize, inputStream);
		}
			break;
		case FOURCC("TPE1"):
		{
			refMetaInfo.artist = ReadID3V2String(dataSize, inputStream);
		}
			break;
		case FOURCC("TRCK"):
		{
			refMetaInfo.trackNumber = (uint8)ReadID3V2String(dataSize, inputStream).ToUInt();
		}
			break;
		case FOURCC("APIC"):
		case FOURCC("COMM"):
		case FOURCC("GEOB"):
		case FOURCC("POPM"):
		case FOURCC("PRIV"):
		case FOURCC("SYLT"):
		case FOURCC("TCON"):
		case FOURCC("TCOP"):
		case FOURCC("TENC"):
		case FOURCC("TPE2"):
		case FOURCC("TPUB"):
		case FOURCC("TSSE"):
		case FOURCC("TXXX"):
		case FOURCC("TYER"):
		case FOURCC("WOAF"):
		case FOURCC("WOAR"):
		case FOURCC("WOAS"):
		{
			inputStream.Skip(dataSize);
		}
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	return dataSize + 10;
}

//
void ReadID3V2Tags(SeekableInputStream& inputStream, SMetaInfo& metaInfo)
{
	byte flags;
	byte id3Identifier[3];
	uint16 version;
	int32 leftSize;
	uint32 totalTagSize, frameSize;

	DataReader dataReader(false, inputStream);

	dataReader.ReadBytes(id3Identifier, 3);
	if(MemCmp(id3Identifier, "ID3", 3) == 0)
	{
		version = dataReader.ReadUInt16();
		flags = dataReader.ReadByte();
		totalTagSize = ((dataReader.ReadByte() & 0x7F) << 21) | ((dataReader.ReadByte() & 0x7F) << 14) | ((dataReader.ReadByte() & 0x7F) << 7) | (dataReader.ReadByte() & 0x7F);

		leftSize = totalTagSize;
		while(leftSize > 0)
		{
			frameSize = ReadID3V2Frame(inputStream, metaInfo);
			leftSize -= frameSize;

			if(frameSize == 0)
			{
				//we reached end of tags... rest is padding
				inputStream.Skip(leftSize);
				break;
			}
		}
	}
	else
	{
		inputStream.Rewind(3);
	}
}
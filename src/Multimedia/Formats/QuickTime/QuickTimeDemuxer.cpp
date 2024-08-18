/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
#include "QuickTimeDemuxer.hpp"

//Public methods
void QuickTimeDemuxer::ReadHeader()
{
	AtomHeader atomHeader;
	while(this->ReadNextAtomHeader(this->inputStream, atomHeader))
	{
		switch(atomHeader.atomType)
		{
			case FOURCC(u8"ftyp"):
				this->ReadFileTypeAtom(atomHeader.DataReader());
				break;
			case FOURCC(u8"moov"):
				this->ReadMoovAtom(atomHeader.DataReader());
				break;
			case FOURCC(u8"free"):
			case FOURCC(u8"mdat"):
			case FOURCC(u8"wide"):
				this->inputStream.Skip(atomHeader.dataSize);
				break;
			default:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	}
}

void QuickTimeDemuxer::Seek(uint64 timestamp, const class TimeScale &timeScale)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

//Private methods
void QuickTimeDemuxer::ReadFileTypeAtom(DataReader& dataReader)
{
	switch(this->ReadFourCC(dataReader))
	{
		case FOURCC(u8"isom"):
		case FOURCC(u8"M4A "):
			this->type = Type::IsoBase;
			break;
		case FOURCC(u8"qt  "):
			this->type = Type::QuickTime;
			break;
	}
	dataReader.Skip(4); //minor version

	while(!dataReader.InputStream().IsAtEnd())
		dataReader.Skip(4); //compatible brands
}

void QuickTimeDemuxer::ReadMoovAtom(DataReader &dataReader)
{
	AtomHeader atomHeader;
	while(this->ReadNextAtomHeader(dataReader, atomHeader))
	{
		switch(atomHeader.atomType)
		{
			case FOURCC(u8"mvhd"):
			case FOURCC(u8"trak"):
				dataReader.Skip(atomHeader.dataSize);
				break;
			case FOURCC(u8"udta"):
				this->ReadUserDataAtom(atomHeader.DataReader());
				break;
			default:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	}
}

bool QuickTimeDemuxer::ReadNextAtomHeader(InputStream& inputStream, QuickTimeDemuxer::AtomHeader& atomHeader)
{
	if(inputStream.IsAtEnd())
		return false;

	DataReader dataReader(true, inputStream);

	uint32 atomSize = dataReader.ReadUInt32();
	atomHeader.atomType = this->ReadFourCC(inputStream);

	atomHeader.dataSize = atomSize - 8;
	atomHeader.dataInputStream = new LimitedInputStream(inputStream, atomHeader.dataSize);
	atomHeader.dataReader = new DataReader(true, *atomHeader.dataInputStream);

	return true;
}

void QuickTimeDemuxer::ReadMetaAtom(DataReader &dataReader)
{
	if(this->type == Type::IsoBase)
	{
		uint32 version = dataReader.ReadUInt32();
		ASSERT_EQUALS(0, version);
	}

	AtomHeader atomHeader;
	while(this->ReadNextAtomHeader(dataReader, atomHeader))
	{
		switch(atomHeader.atomType)
		{
			case FOURCC(u8"hdlr"):
				dataReader.Skip(atomHeader.dataSize);
				break;
			case FOURCC(u8"ilst"):
				this->ReadMetaDataListAtom(atomHeader.DataReader());
				break;
			default:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	}
}

void QuickTimeDemuxer::ReadMetaDataListAtom(DataReader& dataReader)
{
	AtomHeader atomHeader;
	while(this->ReadNextAtomHeader(dataReader, atomHeader))
	{
		String tagName = this->AtomTypeToString(atomHeader.atomType);

		AtomHeader dataAtom;
		this->ReadNextAtomHeader(dataReader, dataAtom);
		ASSERT_EQUALS(FOURCC(u8"data"), dataAtom.atomType);

		uint32 type = dataReader.ReadUInt32();
		uint32 locale = dataReader.ReadUInt32();
		switch(type)
		{
			case 1:
			{
				TextReader utf8Reader(inputStream, TextCodecType::UTF8);
				String value = utf8Reader.ReadStringBySize(dataAtom.dataSize - 8);
				this->metaInfo.additionalTags.Insert(tagName, value);
			}
			break;
			default:
				NOT_IMPLEMENTED_ERROR;
		}
	}
}

UniquePointer<IPacket> QuickTimeDemuxer::ReadPacket()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return UniquePointer<IPacket>();
}

void QuickTimeDemuxer::ReadUserDataAtom(DataReader &dataReader)
{
	AtomHeader atomHeader;
	while(this->ReadNextAtomHeader(dataReader, atomHeader))
	{
		switch(atomHeader.atomType)
		{
			case FOURCC(u8"meta"):
				this->ReadMetaAtom(atomHeader.DataReader());
			break;
			case FOURCC4(0xA9, u8's', u8'w', u8'r'):
			{
				String tagName = this->AtomTypeToString(atomHeader.atomType);

				uint16 size = dataReader.ReadUInt16();
				uint16 type = dataReader.ReadUInt16();
				if(type > 0x400)
				{
					uint8 c[3];
					c[0] = static_cast<uint8>((type >> 10) + 0x60);
					c[1] = static_cast<uint8>(((type >> 5) & 0x1F) + 0x60);
					c[2] = static_cast<uint8>((type & 0x1F) + 0x60);

					TextReader utf8Reader(dataReader.InputStream(), TextCodecType::UTF8);
					String value = utf8Reader.ReadStringBySize(size);
					this->metaInfo.additionalTags.Insert(tagName, value);
				}
				else
					NOT_IMPLEMENTED_ERROR; //TODO: implement me
			}
			break;
			default:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	}
}

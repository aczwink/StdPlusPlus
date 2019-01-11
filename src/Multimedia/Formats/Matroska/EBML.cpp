/*
* Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
//Corresponding header
#include "EBML.hpp"
//Local
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>
#include "EBMLMasterReader.hpp"
//Namespaces
using namespace StdXX;

//Local inline fnctions
inline uint64 PutLength(uint64 uncoded, uint8 length)
{
	return uncoded | (1ULL << uint64((length - 1) * 8 + (8 - length)));
}

//Namespace functions
uint64 EBML::DecodeVariableLengthInteger(uint8 &length, InputStream &inputStream, bool isElementSize)
{
	byte first, mask;
	uint64 result;

	DataReader reader(true, inputStream);

	result = 0;
	length = 1;
	mask = 0x80;
	first = reader.ReadByte();
	while (mask)
	{
		if (isElementSize && (first == Unsigned<byte>::Max()))
			return Unsigned<uint64>::Max(); //live recording

		if (first & mask)
		{
			uint8 firstBitLength = 8 - length;
			uint8 restBitLength = (length - 1) * 8;

			first &= (1 << firstBitLength) - 1;
			return result | ((uint64)first << restBitLength);
		}

		result = (result << 8) | reader.ReadByte();
		length++;
		mask >>= 1;
	}

	NOT_IMPLEMENTED_ERROR;
	return result;
}

void EBML::ParseElementHeader(Element &element, InputStream &inputStream)
{
	//read id
	uint8 length;
	element.id = DecodeVariableLengthInteger(length, inputStream);
	element.id = PutLength(element.id, length);
	element.headerSize = length;

	//read size
	element.dataSize = DecodeVariableLengthInteger(length, inputStream, true);
	element.headerSize += length;

	//check if we can know an offset
	SeekableInputStream* seekableInputStream = dynamic_cast<SeekableInputStream*>(&inputStream);
	if (seekableInputStream)
		element.dataOffset = seekableInputStream->GetCurrentOffset();
	else
		element.dataOffset = Unsigned<uint64>::Max();

	element.dataType = DataType::Unknown;
}

bool EBML::ParseHeader(Header &header, SeekableInputStream &inputStream)
{
	Element ebmlElement;
	ParseElementHeader(ebmlElement, inputStream);
	if (ebmlElement.id != EBML_ID_EBML)
		return false;
	
	MasterReader reader(ebmlElement, inputStream);
	while (reader.HasMore())
	{
		Element child;
		reader.ReadNextChildHeader(child);
		
		//set type
		switch(child.id)
		{
		case EBML_ID_DOCTYPE:
			child.dataType = DataType::ASCII_String;
			break;
		case EBML_ID_DOCTYPEVERSION:
		case EBML_ID_DOCTYPEREADVERSION:
			child.dataType = DataType::UInt;
			break;
		}

		reader.ReadCurrentChildData(child);

		//set attributes
		switch (child.id)
		{
		case EBML_ID_DOCTYPE:
			header.docType = child.data.string;
			break;
		case EBML_ID_DOCTYPEREADVERSION:
			header.docTypeVersion = child.data.ui;
			break;
		case EBML_ID_DOCTYPEVERSION: //not interesting for reading
			break;
		}
	}
	reader.Verify();
	
	return true;
}

void EBML::ReadElementData(Element &element, InputStream &inputStream)
{
	switch (element.id)
	{
	case EBML_ID_CRC32:
	{
		DataReader reader(false, inputStream); //the crc32 is stored little endian
		element.data.ui = reader.ReadUInt32();
		return;
	}
	break;
	}

	switch(element.dataType)
	{
		case DataType::Unknown:
		case DataType::Binary: //we never read binary data directly, because it can be assumed that this is large
		case DataType::Date: //not yet implemented
		case DataType::Master: //we never read master data directly
		case DataType::UTF8: //not yet implemented
			inputStream.Skip(static_cast<uint32>(element.dataSize));
			break;
		case DataType::ASCII_String:
			{
				TextReader reader(inputStream, TextCodecType::ASCII);

				element.data.string = reader.ReadString(static_cast<uint32>(element.dataSize)); //size=length here because for ascii each char is 1 byte
			}
			break;
		case DataType::Float:
			{
				DataReader reader(true, inputStream);

				if (element.dataSize == 4)
					element.data.f = reader.ReadFloat32();
				else
					element.data.f = reader.ReadFloat64();
			}
			break;
		case DataType::UInt:
			{
				DataReader reader(true, inputStream);

				element.data.ui = 0;
				uint8 left = (uint8)element.dataSize;
				while (left--)
					element.data.ui = (element.data.ui << 8) | reader.ReadByte();
			}
			break;

		/*
		 * case EBMLType::Master:
		{
			if (this->BeginParseChildren(id))
			{
				uint64 left = size;
				while (left)
					left -= this->ParseElement();
			}
			else
			{
				this->inputStream.Skip(size);
			}

			this->EndParseChildren(id);
		}
		break;
		 */
	}
}
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
//Class header
#include "EBMLParser.hpp"
//Local
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>
#include <Std++/Debug.hpp>
//Namespaces
using namespace StdXX;

//Constructor
EBMLParser::EBMLParser(InputStream &inputStream) : inputStream(inputStream), ebmlElementFound(false)
{
	//read ebml element
	this->ParseElement();
	ASSERT(this->ebmlElementFound, u8"Invalid ebml file.");
}

//Private methods
uint64 EBMLParser::DecodeVariableLengthInteger(uint8 &refLength)
{
	byte first, mask;
	uint64 result;

	DataReader reader(true, this->inputStream);

	result = 0;
	refLength = 1;
	mask = 0x80;
	first = reader.ReadByte();
	while (mask)
	{
		if (first & mask)
		{
			uint8 firstBitLength, restBitLength;

			firstBitLength = 8 - refLength;
			restBitLength = (refLength - 1) * 8;

			first &= (1 << firstBitLength) - 1;
			return result | ((uint64)first << restBitLength);
		}

		result = (result << 8) | reader.ReadByte();
		refLength++;
		mask >>= 1;
	}

	NOT_IMPLEMENTED_ERROR;
	return result;
}

bool EBMLParser::GetElementInfo(uint64 id, EBMLElementInfo &elementInfo) const
{
	switch (id)
	{
	case EBML_ID_EBML:
		elementInfo.type = EBMLType::Master;
		return true;
	}
	return false;
}

void EBMLParser::ParseEBMLHeader(EBMLHeader &header)
{
}

uint64 EBMLParser::ParseElement()
{
	uint64 id, size;
	uint8 headerLength = this->ParseElementHeader(id, size);
	if (this->inputStream.IsAtEnd())
		return 0;

	DataReader reader(true, this->inputStream);

	EBMLElementInfo elementInfo;
	if (this->GetElementInfo(id, elementInfo))
	{
		switch (elementInfo.type)
		{
		case EBMLType::ASCII_String:
		{
			TextReader reader(this->inputStream, TextCodecType::ASCII);

			this->ParseString(id, reader.ReadString(size)); //size=length here because for ascii each char is 1 byte
		}
		break;
		case EBMLType::Binary:
		{
			this->ParseBinary(id, size);
		}
		break;
		case EBMLType::Date:
		{
			this->inputStream.Skip(size);
		}
		break;
		case EBMLType::Float:
		{
			float64 value;

			if (size == 4)
				value = reader.ReadFloat32();
			else
				value = reader.ReadFloat64();

			this->ParseFloat(id, value);
		}
		break;
		case EBMLType::Master:
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
		case EBMLType::UInt:
		{
			uint8 left;
			uint64 value;

			value = 0;
			left = (uint8)size;
			while (left--)
				value = (value << 8) | reader.ReadByte();

			this->ParseUInt(id, value);
		}
		break;
		case EBMLType::UTF8:
		{
			this->inputStream.Skip(size);
		}
		break;
		default:
			NOT_IMPLEMENTED_ERROR;
		}
	}
	else
	{
		this->inputStream.Skip(size);
	}

	return headerLength + size;
}

uint8 EBMLParser::ParseElementHeader(uint64 &id, uint64 &size)
{
	//read id
	uint8 length;
	id = this->DecodeVariableLengthInteger(length);
	id = this->PutLength(id, length);
	uint8 headerLength = length;

	//read size
	size = this->DecodeVariableLengthInteger(length);
	headerLength += length;

	return headerLength;
}

void EBMLParser::ParseString(uint64 id, const String &string)
{
}
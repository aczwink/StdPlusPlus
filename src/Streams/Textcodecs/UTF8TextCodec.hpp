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
#include <Std++/Streams/TextCodec.hpp>
//Local
#include <Std++/Debug.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Streams/Writers/DataWriter.hpp>
//Namespaces
using namespace StdXX;

class UTF8TextCodec : public TextCodec
{
public:
	//Methods
	uint32 ReadCodePoint(InputStream &inputStream) const override
	{
		DataReader reader(true, inputStream);
		uint32 b1 = reader.ReadByte();
		if(b1 & 0x80)
		{
			if(b1 & 0x20)
			{
				//3 or 4 bytes
				NOT_IMPLEMENTED_ERROR;
			}

			return ((b1 & 0x1F) << 6) | (reader.ReadByte() & 0x3F);
		}

		return b1;
	}

	void WriteCodePoint(uint32 codePoint, OutputStream &outputStream) const override
	{
		DataWriter dataWriter(true, outputStream);
		if(codePoint <= 0x7F)
		{
			//ascii -> map directly
			dataWriter.WriteByte(static_cast<byte>(codePoint & 0xFF));
		}
		else if(codePoint <= 0x7FF)
		{
			//2 bytes
			/*
			first byte:
			7 -> 1
			6 -> 1
			5 -> 0
			4...0 -> bits 10 to 6 of codepoint
			*/
			dataWriter.WriteByte(static_cast<byte>(0xC0 | (codePoint >> 6)));

			/*
			second byte:
			7 -> 1
			6 -> 0
			5...0 -> bits 5 to 0 of codepoint
			*/
			dataWriter.WriteByte(static_cast<byte>(0x80 | (codePoint & 0x3F)));
		}
		else if(codePoint <= 0xFFFF)
		{
			//3 bytes
			dataWriter.WriteByte(static_cast<byte>(0xE0 | (codePoint >> 12)));
			dataWriter.WriteByte(static_cast<byte>(0x80 | ((codePoint >> 6) & 0x3F)));
			dataWriter.WriteByte(static_cast<byte>(0x80 | (codePoint & 0x3F)));
		}
		else if(codePoint <= 0x10FFFF)
		{
			//4 bytes
			NOT_IMPLEMENTED_ERROR;
		}
		else
		{
			NOT_IMPLEMENTED_ERROR; //illegal code point
		}
	}
};
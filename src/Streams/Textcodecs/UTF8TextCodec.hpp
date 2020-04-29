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
	uint32 ReadCodePoint(InputStream &inputStream, uint8& nBytesRead) const override
	{
		DataReader reader(true, inputStream);

		uint8 b1 = reader.ReadByte();
		if(b1 & 0x80_u8)
		{
			//2 bytes or more...
			uint8 b2 = reader.ReadByte();
			ASSERT((b2 & 0xC0_u8) == 0x80_u8, u8"Illegal encoding.");

			if(b1 & 0x20_u8)
			{
				//3 or 4 bytes...
				uint8 b3 = reader.ReadByte();
				ASSERT((b3 & 0xC0_u8) == 0x80_u8, u8"Illegal encoding.");

				if(b1 & 0x10_u8)
				{
					//4 bytes
					NOT_IMPLEMENTED_ERROR;
				}

				//3 byte
				ASSERT((b1 & 0xF0_u8) == 0xE0_u8, u8"Illegal encoding.");
				nBytesRead = 3;
				return (uint32(b1 & 0xF_u8) << 12_u32) | (uint32(b2 & 0x3F_u8) << 6_u32) | (b3 & 0x3F_u8);
			}

			//2 byte
			ASSERT((b1 & 0x60_u8) == 0x40_u8, u8"Illegal encoding.");
			nBytesRead = 2;
			return (uint32(b1 & 0x1F_u8) << 6_u32) | (b2 & 0x3F_u8);
		}

		nBytesRead = 1;
		return b1;
	}

	/*
	if(b[0] & 0x20)
		{
			if(b[0] & 0x10)
			{
				//4 byte
				this->inputStream.ReadBytes(&b[1], 3);
				codePoint = (uint32) ((b[0] & 0xF) << 18);
				codePoint |= (b[1] & 0x3F) << 12;
				codePoint |= (b[2] & 0x3F) << 6;
				codePoint |= b[3] & 0x3F;
			}
			else
			{
				//3 byte
				this->inputStream.ReadBytes(&b[1], 2);
				codePoint = (uint32) ((b[0] & 0xF) << 12);
				codePoint |= (b[1] & 0x3F) << 6;
				codePoint |= b[2] & 0x3F;
			}
		}
		*/

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
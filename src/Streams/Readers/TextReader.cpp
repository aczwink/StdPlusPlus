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
//Class Header
#include <Std++/Streams/Readers/TextReader.hpp>
//Local
#include <Std++/Char.hpp>
//Namespaces
using namespace StdXX;

//Operators
TextReader &TextReader::operator>>(uint32 &i)
{
	String tmp;
	*this >> tmp;
	i = tmp.ToUInt32();

	return *this;
}

TextReader &TextReader::operator>>(float32 &f)
{
	String tmp;
	*this >> tmp;
	f = tmp.ToFloat32();

	return *this;
}

TextReader &TextReader::operator>>(String &target)
{
	target = String();

	uint32 codePoint = this->SkipWhitespaces();
	if(codePoint == Natural<uint32>::Max())
		return *this;
	target += codePoint;

	while(true)
	{
		codePoint = this->codec->ReadCodePoint(this->inputStream);
		if(this->inputStream.IsAtEnd())
			break;

		if(IsWhiteSpaceChar(codePoint))
			break;
		target += codePoint;
	}

	return *this;
}

//Public methods
String TextReader::ReadString(uint32 length)
{
	String result;
	while(!this->inputStream.IsAtEnd() && length--)
	{
		uint32 codePoint = this->codec->ReadCodePoint(this->inputStream);
		result += codePoint;
	}

	return result;
}

ByteString TextReader::ReadASCII_Line()
{
    char c;
    ByteString buffer;

    while(!this->inputStream.IsAtEnd())
    {
		this->dataReader >> c;
        if(c == '\n')
            break;
        if(c == '\r')
            continue;
        buffer += c;
    }

    return buffer;
}

ByteString TextReader::ReadASCII_ZeroTerminated()
{
    char c;
    ByteString buffer;

    while(!this->inputStream.IsAtEnd())
    {
		this->dataReader >> c;
        if(c == 0)
            break;
        buffer += c;
    }

    return buffer;
}

uint32 TextReader::ReadUTF8()
{
    byte b[4];
    uint32 codePoint;

	this->dataReader >> b[0];
	if(b[0] & 0x80)
    {
        //more than one byte
        ASSERT(b[0] & 0x40, "If you see this, report to StdXX"); //this is always set for more than one byte

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
        else
        {
            //2 bytes
			this->dataReader >> b[1];
            codePoint = (uint32) ((b[0] & 0x1F) << 6);
            codePoint |= b[1] & 0x3F;
        }
    }
    else
    {
        //ascii
        codePoint = b[0];
    }

    return codePoint;
}

UTF8String TextReader::ReadUTF8Line()
{
    uint32 c;
    UTF8String result;

    while(true)
    {
        c = this->ReadUTF8();
        if(this->inputStream.IsAtEnd() || c == '\n')
            break;
        if(c == '\r')
            continue;

        result += c;
    }

    return result;
}

//Private methods
uint32 TextReader::SkipWhitespaces()
{
	uint32 codePoint;
	while(true)
	{
		if(this->inputStream.IsAtEnd())
			return Natural<uint32>::Max();
		codePoint = this->codec->ReadCodePoint(this->inputStream);
		if(!IsWhiteSpaceChar(codePoint))
			break;
	}

	return codePoint;
}
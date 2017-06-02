/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class Header
#include <ACStdLib/Streams/Readers/TextReader.hpp>
//Local
#include <ACStdLib/Debug.h>
#include <ACStdLib/Memory.h>
#include <ACStdLib/Containers/Strings/StringUtil.h>
//Namespaces
using namespace ACStdLib;

//Operators
TextReader &TextReader::operator>>(uint32 &i)
{
	ByteString tmp;

	*this >> tmp;
	i = (uint32)StringToUInt64(tmp);

	return *this;
}

TextReader &TextReader::operator>>(float32 &f)
{
	ByteString tmp;

	*this >> tmp;
	f = (float32)tmp.ToFloat();

	return *this;
}

TextReader &TextReader::operator>>(ByteString &target)
{
	byte b;

	b = this->SkipWhitespaces();
	if(this->inputStream.IsAtEnd())
	{
		target = ByteString();
		return *this;
	}

	target = b;
	while(true)
	{
		this->dataReader >> b;
		if(this->IsWhitespace(b))
			break;
		target += b;

		if(this->inputStream.IsAtEnd())
			break;
	}

	return *this;
}

//Private methods
bool TextReader::IsWhitespace(byte b)
{
	switch(b)
	{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			return true;
	}

	return false;
}

byte TextReader::SkipWhitespaces()
{
	byte b;

	while(true)
	{
		if(this->inputStream.IsAtEnd())
			return -1;
		this->dataReader >> b;
		if(!this->IsWhitespace(b))
			break;
	}

	return b;
}

//Public methods
ByteString TextReader::ReadASCII(uint32 length)
{
    char c;
    ByteString buffer;

    while(!this->inputStream.IsAtEnd() && length--)
    {
		this->dataReader >> c;
        buffer += c;
    }

    return buffer;
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
        ASSERT(b[0] & 0x40); //this is always set for more than one byte

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
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
	if(this->inputStream.HitEnd())
	{
		target = ByteString();
		return *this;
	}

	target = b;
	while(true)
	{
		b = this->inputStream.ReadByte();
		if(this->inputStream.HitEnd())
			break;
		if(this->IsWhitespace(b))
			break;

		target += b;
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
		b = this->inputStream.ReadByte();
		if(this->inputStream.HitEnd())
			return -1;
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

    while(length--)
    {
        c = this->inputStream.ReadByte();
        if(this->inputStream.HitEnd())
            break;
        buffer += c;
    }

    return buffer;
}

ByteString TextReader::ReadASCII_Line()
{
    char c;
    ByteString buffer;

    while(true)
    {
        c = this->inputStream.ReadByte();
        if(c == '\n' || this->inputStream.HitEnd())
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

    while(true)
    {
        c = this->inputStream.ReadByte();
        if(c == 0 || this->inputStream.HitEnd())
            break;
        buffer += c;
    }

    return buffer;
}

uint32 TextReader::ReadUTF8()
{
    byte b;
    uint32 codePoint;

    b = this->inputStream.ReadByte();

    if(b & 0x80)
    {
        //more than one byte
        ASSERT(b & 0x40); //this is always set for more than one byte

        if(b & 0x20)
        {
            if(b & 0x10)
            {
                //4 byte
                codePoint = (b & 0xF) << 18;
                codePoint |= (this->inputStream.ReadByte() & 0x3F) << 12;
                codePoint |= (this->inputStream.ReadByte() & 0x3F) << 6;
                codePoint |= this->inputStream.ReadByte() & 0x3F;
            }
            else
            {
                //3 byte
                codePoint = (b & 0xF) << 12;
                codePoint |= (this->inputStream.ReadByte() & 0x3F) << 6;
                codePoint |= this->inputStream.ReadByte() & 0x3F;
            }
        }
        else
        {
            //2 bytes
            codePoint = (b & 0x1F) << 6;
            codePoint |= this->inputStream.ReadByte() & 0x3F;
        }
    }
    else
    {
        //ascii
        codePoint = b;
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
        if(this->inputStream.HitEnd() || c == '\n')
            break;
        if(c == '\r')
            continue;

        result += c;
    }

    return result;
}
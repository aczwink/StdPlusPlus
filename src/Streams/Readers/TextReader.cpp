/*
 * Copyright (c) 2017-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Strings/String.hpp>
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
	NOT_IMPLEMENTED_ERROR; //TODO: reimplement me
	//f = tmp.ToFloat32();

	return *this;
}

TextReader &TextReader::operator>>(String &target)
{
	target = String();

	uint32 codePoint = this->SkipWhitespaces();
	if(codePoint == Unsigned<uint32>::Max())
		return *this;
	target += codePoint;

	while(true)
	{
		uint8 nBytesRead;
		codePoint = this->codec->ReadCodePoint(this->inputStream, nBytesRead);
		if(this->inputStream.IsAtEnd())
			break;

		if(IsWhiteSpaceChar(codePoint))
			break;
		target += codePoint;
	}

	return *this;
}

//Public methods
String TextReader::ReadLine()
{
	bool nextShouldBeNewline = false;
	String buffer;
	while(!this->inputStream.IsAtEnd())
	{
		uint8 nBytesRead;
		uint32 codePoint = this->codec->ReadCodePoint(this->inputStream, nBytesRead);
		if(codePoint == u8'\n')
			break;
		if(codePoint == u8'\r')
		{
			nextShouldBeNewline = true;
			continue;
		}
		if(nextShouldBeNewline)
		{
			nextShouldBeNewline = false;
			buffer += u8'\r';
		}
		buffer += codePoint;
	}

	return buffer;
}

String TextReader::ReadString(uint32 length)
{
	String result;
	while(!this->inputStream.IsAtEnd() && length--)
	{
		uint8 nBytesRead;
		uint32 codePoint = this->codec->ReadCodePoint(this->inputStream, nBytesRead);
		result += codePoint;
	}

	return result;
}

String TextReader::ReadStringBySize(uint32 size)
{
	String result;
	while(!this->inputStream.IsAtEnd() && (size > 0))
	{
		uint8 nBytesRead;
		uint32 codePoint = this->codec->ReadCodePoint(this->inputStream, nBytesRead);
		ASSERT(nBytesRead <= size, u8"REPORT THIS PLEASE!");
		size -= nBytesRead;

		result += codePoint;
	}

	return result;
}

String TextReader::ReadZeroTerminatedString()
{
	String result;
	while (!this->inputStream.IsAtEnd())
	{
		uint8 nBytesRead;
		uint32 codePoint = this->codec->ReadCodePoint(this->inputStream, nBytesRead);
		if (codePoint == 0)
			break;
		result += codePoint;
	}

	return result;
}

String TextReader::ReadZeroTerminatedString(uint32 length)
{
	String result;
	while (!this->inputStream.IsAtEnd() && length)
	{
		uint8 nBytes;
		uint32 codePoint = this->codec->ReadCodePoint(this->inputStream, nBytes);
		length--;
		if (codePoint == 0)
			break;
		result += codePoint;
	}
	this->inputStream.Skip(length); //skip the remaining bytes

	return result;
}

String TextReader::ReadZeroTerminatedStringBySize(uint32 size)
{
	String result;
	while(!this->inputStream.IsAtEnd() && (size > 0))
	{
		uint8 nBytesRead;
		uint32 codePoint = this->codec->ReadCodePoint(this->inputStream, nBytesRead);
		ASSERT(nBytesRead <= size, u8"REPORT THIS PLEASE!");
		size -= nBytesRead;

		if(codePoint == 0)
			break;

		result += codePoint;
	}
	this->inputStream.Skip(size); //skip remaining bytes

	return result;
}

//Private methods
uint32 TextReader::SkipWhitespaces()
{
	uint32 codePoint;
	while(true)
	{
		if(this->inputStream.IsAtEnd())
			return Unsigned<uint32>::Max();
		uint8 nBytesRead;
		codePoint = this->codec->ReadCodePoint(this->inputStream, nBytesRead);
		if(!IsWhiteSpaceChar(codePoint))
			break;
	}

	return codePoint;
}
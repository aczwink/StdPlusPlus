/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
//Class header
#include <ACStdLib/Containers/Strings/String.hpp>
//Local
#include <ACStdLib/Containers/Strings/ConstStringIterator.hpp>
//Namespaces
using namespace ACStdLib;

//Operators
String &String::operator+=(uint32 codePoint)
{
	this->Detach();

	this->ResizeAdditional(4);
	uint8 nBytes = this->Encode(codePoint, &this->sharedResource->data[this->sharedResource->nElements]);
	this->sharedResource->nElements += nBytes;
	this->size += nBytes;
	this->length++;
}

String &String::operator+=(const String &rhs)
{
	if(this->IsNull())
	{
		*this = rhs;
		return *this;
	}

	//either both UTF8 or both UTF16
	if(this->IsUTF8() != rhs.IsUTF8())
		this->ToUTF16();

	this->Detach();
	this->ResizeAdditional(rhs.size);
	MemCopy(&this->sharedResource->data[this->sharedResource->GetNumberOfElements()], rhs.data, rhs.size);
	this->sharedResource->nElements += rhs.size;
	this->size += rhs.size;
	this->length += rhs.length;

	return *this;
}

bool String::operator==(const String &rhs) const
{
	if(this->length != rhs.length)
		return false;

	auto it = this->begin();
	auto it2 = rhs.begin();
	for(uint32 i = 0; i < this->length; i++)
	{
		if(*it != *it2)
			return false;
		++it;
		++it2;
	}

	return true;
}

bool String::operator<(const String &rhs) const
{
	uint32 cmpLength = MIN(this->length, rhs.length);
	cmpLength++; //include the null byte in comparison, because this will prefer prefixes to be less

	auto it = this->begin();
	auto it2 = rhs.begin();
	while(cmpLength--)
	{
		if(*it < *it2)
			return true;
		else if(*it > *it2)
			return false;
	}

	return false; //only case that is left, is that both strings are equal
}

//Public methods
uint32 String::Find(const String &string, uint32 startPos, uint32 length) const
{
	auto it = this->begin();

	//skip to startPos first
	while(startPos--)
		++it;

	while(length)
	{
		auto it2 = string.begin();

		//try to match first code point
		while(*it != *it2)
		{
			++it;
			length--;

			if(!length)
				return Natural<uint32>::Max();
		}

		//found first code point, check for rest
		if(length < string.length)
			break; //can't find it anymore

		if(it.Equals(it2, string.length))
			return it.GetPosition();

		//no match, skip current first char
		++it;
		length--;
	}

	return Natural<uint32>::Max();
}

uint32 String::FindReverse(const String &string, uint32 startPos, uint32 length) const
{
	startPos = MIN(startPos, this->length);
	auto it = --this->end();

	//move to correct position first
	while(this->length > startPos)
	{
		++it;
		startPos++;
	}

	while(length)
	{
		auto it2 = --string.end();

		//try to match last code point
		while(*it != *it2)
		{
			--it;
			length--;

			if(!length)
				return Natural<uint32>::Max();
		}

		//found last code point, check for rest
		if(length < string.length)
			break; //can't find it anymore

		if(it.EqualsReversed(it2, string.length))
			return it.GetPosition();

		//no match, skip current first char
		--it;
		length--;
	}

	return Natural<uint32>::Max();
}

bool String::StartsWith(const String &string) const
{
	if(string.length > this->length)
		return false;

	return this->begin().Equals(string.begin(), string.length);
}

String String::SubString(uint32 startPos, uint32 length) const
{
	auto it = this->GetIteratorAt(startPos);
	uint32 startOffset = it.GetByteOffset();

	//go to end
	for(uint32 i = 0; i < length; i++)
		++it;
	uint32 size = it.GetByteOffset() - startOffset;

	String result = *this;
	result.length = length;
	result.size = size;
	result.data += startOffset;

	return result;
}

const String &String::ToUTF8() const
{
	if(!this->IsUTF8())
	{
		NOT_IMPLEMENTED_ERROR; //TODO: impelment me
	}

	return *this;
}

//Private methods
uint32 String::DecodeUTF8(const uint8 *src, uint8 &nBytes) const
{
	uint32 b1;

	b1 = static_cast<uint32>(*src++);

	if(b1 & 0x80)
	{
		if(b1 & 0x20)
		{
			//3 or 4 bytes
			ASSERT(0);
		}

		nBytes = 2;

		return ((b1 & 0x1F) << 6) | (*src & 0x3F);
	}

	nBytes = 1;

	return b1;
}

uint32 String::DecodeUTF16(const uint16 *src, bool &isSurrogate) const
{
	if(*src > 0xD800)
	{
		ASSERT(0); //surrogate-pair
	}

	isSurrogate = false;
	return static_cast<uint32>(*src);
}

void String::Detach() const
{
	if(this->sharedResource)
	{
		if(this->sharedResource->referenceCounter != 1)
		{
			this->sharedResource->referenceCounter--;
			this->sharedResource = this->sharedResource->Copy();
			this->data = this->sharedResource->data;
		}
	}
	else
	{
		this->sharedResource = new Resource;

		if(this->data) //we have a literal, copy it now
		{
			this->sharedResource->EnsureCapacity(this->size);
			this->sharedResource->nElements = this->size;
			MemCopy(this->sharedResource->data, this->data, this->size);
			this->data = this->sharedResource->data;
		}
	}
}

uint8 String::EncodeUTF8(uint32 codePoint, byte *dest) const
{
	if(codePoint <= 0x7F)
	{
		//ascii -> map directly
		*dest = static_cast<byte>(codePoint & 0xFF);
		return 1;
	}

	if(codePoint <= 0x7FF)
	{
		//2 bytes
		/*
		first byte:
		7 -> 1
		6 -> 1
		5 -> 0
		4...0 -> bits 10 to 6 of codepoint
		*/
		*dest++ = static_cast<byte>(0xC0 | (codePoint >> 6));

		/*
		second byte:
		7 -> 1
		6 -> 0
		5...0 -> bits 5 to 0 of codepoint
		*/
		*dest++ = static_cast<byte>(0x80 | (codePoint & 0x3F));

		return 2;
	}

	if(codePoint <= 0xFFFF)
	{
		//3 bytes
		*dest++ = static_cast<byte>(0xE0 | (codePoint >> 12));
		*dest++ = static_cast<byte>(0x80 | ((codePoint >> 6) & 0x3F));
		*dest++ = static_cast<byte>(0x80 | (codePoint & 0x3F));

		return 3;
	}

	if(codePoint <= 0x10FFFF)
	{
		//4 bytes
		ASSERT(0);
	}

	NOT_IMPLEMENTED_ERROR; //illegal code point
	return 0;
}

ConstStringIterator String::GetIteratorAt(uint32 startPos) const
{
	if(startPos > this->length / 2)
	{
		auto it = this->end();
		while(startPos++ < this->length)
			--it;

		return it;
	}

	auto it = this->begin();
	while(startPos--)
		++it;

	return it;
}

//Class functions
String String::CopyRawString(const char *utf8)
{
	String str;

	str.sharedResource = new Resource;
	str.sharedResource->isUTF8 = true;

	str.length = str.CountUTF8Length(reinterpret_cast<const uint8 *>(utf8), str.size);
	str.ResizeAdditional(str.size);

	MemCopy(str.sharedResource->data, utf8, str.size);
	str.sharedResource->nElements = str.size;

	return str;
}

String String::Number(uint64 value, uint8 base, uint8 minLength)
{
	uint64 rest;
	String result, buffer;

	if(value == 0)
		buffer = u8"0";

	while(value > 0)
	{
		rest = value % base;
		value /= base;

		if(rest > 9) //for hex
			buffer += (rest - 10 + u8'A');
		else
			buffer += (rest + u8'0');
	}

	if(buffer.GetLength() < minLength)
	{
		uint8 nCharsToFill;

		nCharsToFill = static_cast<uint8>(minLength - buffer.GetLength());

		for(uint32 i = 0; i < nCharsToFill; i++)
			buffer += (u8'0');
	}

	result.Detach();
	result.ResizeAdditional(buffer.sharedResource->nElements);

	auto it = buffer.end();
	for(uint32 i = 0; i < buffer.GetLength(); i++)
	{
		--it;
		result += (*it);
	}

	return result;
}

//For range-based loop
ConstStringIterator String::begin() const
{
	return {*this, this->data, 0};
}

ConstStringIterator String::end() const
{
	return {*this, this->data + this->size, this->length};
}
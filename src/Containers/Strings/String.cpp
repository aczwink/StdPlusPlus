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
#include <Std++/Containers/Strings/String.hpp>
//Global
#include <cstdio>
//Local
#include <Std++/Containers/Strings/ConstStringIterator.hpp>
#include <Std++/Char.hpp>
#include <Std++/Mathematics.h>
//Namespaces
using namespace StdPlusPlus;

//Operators
String &String::operator+=(uint32 codePoint)
{
	this->Detach();

	this->ResizeAdditional(4);
	uint8 nBytes = this->Encode(codePoint, &this->sharedResource->data[this->sharedResource->nElements]);
	this->sharedResource->nElements += nBytes;
	this->size += nBytes;
	this->length++;

	return *this;
}

String &String::operator+=(const String &rhs)
{
	if(this->IsNull())
	{
		*this = rhs;
		return *this;
	}

	this->Detach();

	//either both UTF8 or both UTF16
	if (rhs.IsUTF16() && !this->IsUTF16())
		this->ToUTF16();
	else if (rhs.IsUTF8() && !this->IsUTF8())
		this->ToUTF8();

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

	auto it = this->begin();
	auto it2 = rhs.begin();
	while(cmpLength--)
	{
		if(*it < *it2)
			return true;
		else if(*it > *it2)
			return false;

		++it;
		++it2;
	}

	return this->length < rhs.length; //either left is a prefix or (right is a prefix or both are equal)
}

//Public methods
bool String::EndsWith(const String &string) const
{
	if(string.length > this->length)
		return false;

	return this->end().EqualsReversed(string.end(), string.length + 1); //+1 because the comparison will include the null byte
}

uint32 String::Find(const String &string, uint32 startPos, uint32 length) const
{
	startPos = MIN(startPos, this->length-1);
	length = MIN(length, this->length - startPos);

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
	startPos = MIN(startPos, this->length - 1);
	length = MIN(length, startPos + 1);

	auto it = this->GetIteratorAt(startPos);
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

String String::Replace(const String &from, const String &to) const
{
	if(from.IsEmpty())
		return *this;

	String result;
	result.Detach();
	result.ResizeAdditional(this->size);
	for(auto it = this->begin(); it != this->end();)
	{
		auto it2 = from.begin();
		if(it.Equals(it2, from.length)) //we are at the start of a from
		{
			result += to; //append to
			for(uint32 i = 0; i < from.length; i++) //skip from
				++it;
		}
		else //not at from
		{
			result += *it; //append current char
			++it;
		}
	}

	return result;
}

DynamicArray<String> String::Split(const String &seperator) const
{
	DynamicArray<String> result;

	int32 pos;
	uint32 oldPos = 0;
	while((pos = this->Find(seperator, oldPos)) != Natural<uint32>::Max())
	{
		result.Push(this->SubString(oldPos, pos - oldPos));
		oldPos = pos + seperator.GetLength();
	}

	if(this->GetLength() - oldPos != 0)
		result.Push(this->SubString(oldPos, this->GetLength() - oldPos));

	return result;
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
	length = MIN(length, this->length - startPos);

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

float64 String::ToFloat() const
{
	if(this->data == nullptr || *this->data == 0)
		return 0; //filter out empty strings

	auto it = this->begin();
	//skip leading white spaces
	while(IsWhiteSpaceChar(*it))
		++it;

	//parse sign
	int8 sign = 1;
	if(*it == u8'-')
	{
		++it;
		sign = -1;
	}
	else if (*it == u8'+')
		++it;

	//parse the integer part
	float64 integerPart = 0;
	while(*it)
	{
		if(IN_RANGE(*it, u8'0', u8'9'))
		{
			integerPart = integerPart * 10 + (*it - u8'0');

			++it;
			continue;
		}
		switch(*it)
		{
			case u8'.':
			case u8'e':
			case u8'E':
				goto parseFrac;
				break;
			default:
				//weird symbol
			NOT_IMPLEMENTED_ERROR;
		}
	}

	parseFrac:
	//parse fraction part
	float64 fractionPart = 0;
	uint32 divisor = 1;
	if(*it == u8'.')
	{
		++it;
		while(it != this->end())
		{
			if(IN_RANGE(*it, u8'0', u8'9'))
			{
				fractionPart = fractionPart*10 + (*it - u8'0');
				divisor *= 10;
			}
			else if(*it == u8'e' || *it == u8'E')
			{
				break;
			}
			else
			{
				//weird symbol
				NOT_IMPLEMENTED_ERROR;
			}
			++it;
		}
	}

	//parse exponent
	float64 base = 1;
	if(*it == u8'e' || *it == u8'E')
	{
		++it;

		//parse the exp sign
		int8 expSign = 1;
		if(*it == u8'-')
		{
			++it;
			expSign = -1;
		}
		else if (*it == u8'+')
			++it;

		int16 exp = 0;
		while(*it)
		{
			if(IN_RANGE(*it, u8'0', u8'9'))
			{
				exp = exp * 10 + (*it - u8'0');
			}
			else
			{
				//weird symbol
				NOT_IMPLEMENTED_ERROR;
			}

			++it;
		}

		base = pow(10.0, exp * expSign);
	}

	return sign * (integerPart + fractionPart / divisor) * base;
}

String String::ToLowercase() const
{
	String tmp;

	tmp.sharedResource = new Resource;
	tmp.sharedResource->EnsureCapacity(this->length * 4); //worst case: every code point takes 4 bytes
	for(uint32 codePoint : *this)
	{
		tmp.sharedResource->nElements += this->EncodeUTF8(StdPlusPlus::ToLowercase(codePoint), &tmp.sharedResource->data[tmp.sharedResource->nElements]);
	}
	tmp.data = tmp.sharedResource->data;
	tmp.length = this->length;
	tmp.size = tmp.sharedResource->nElements;

	return tmp;
}

uint64 String::ToUInt() const
{
	auto it = this->begin();

	//skip optional +
	if(*it == u8'+')
		++it;

	uint64 result = 0;
	while(it != this->end())
	{
		if(!IN_RANGE(*it, u8'0', u8'9'))
			NOT_IMPLEMENTED_ERROR;

		result += Power((uint64)10, (uint64)(this->length - it.GetPosition() - 1)) * (*it - u8'0');
		++it;
	}

	return result;
}

const String &String::ToUTF8() const
{
	if(!this->IsUTF8())
	{
		Resource *newRes = new Resource;
		newRes->isUTF8 = true;
		newRes->EnsureCapacity(this->length * 4); //worst-case: every codepoint takes 4 bytes
		uint8 *current = newRes->data;
		uint32 size = 0;
		for (uint32 codePoint : *this)
		{
			uint8 nBytes = this->EncodeUTF8(codePoint, current);
			current += nBytes;
			size += nBytes;
		}

		if (this->sharedResource)
			this->sharedResource->Release();
		this->sharedResource = newRes;
		this->sharedResource->nElements = size;
		this->data = this->sharedResource->data;
		this->size = size;
	}

	return *this;
}

const String &String::ToUTF16() const
{
	if (!this->IsUTF16())
	{
		Resource *newRes = new Resource;
		newRes->isUTF8 = false;
		newRes->EnsureCapacity(this->length * 4); //worst-case: everything is surrogate
		uint16 *current = (uint16 *)newRes->data;
		uint32 newSize = 0;
		for (uint32 codePoint : *this)
		{
			if (this->EncodeUTF16(codePoint, current))
			{
				current++;
				newSize += 2;
			}
			current++;
			newSize += 2;
		}

		if (this->sharedResource)
			this->sharedResource->Release();
		this->sharedResource = newRes;
		this->sharedResource->nElements = newSize;
		this->data = this->sharedResource->data;
		this->size = newSize;
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
			NOT_IMPLEMENTED_ERROR;
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
		NOT_IMPLEMENTED_ERROR; //surrogate-pair
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
			this->sharedResource = this->sharedResource->Copy(this->GetByteOffset(), this->size);
			this->data = this->sharedResource->data;
		}
	}
	else
	{
		this->sharedResource = new Resource;

		if(this->data) //we have a literal, copy it now
		{
			if (this->sharedResource->isUTF8)
			{
				this->sharedResource->EnsureCapacity(this->size);
				MemCopy(this->sharedResource->data, this->data, this->size);
			}
			else
			{
				//encode to utf16
				this->sharedResource->EnsureCapacity(this->length * 4); //worst-case: everything is a surrogate
				uint16 *current = (uint16 *)this->sharedResource->data;
				uint32 targetSize = 0;
				for (uint32 i = 0; i < this->size; )
				{
					uint8 nBytes;
					uint32 codePoint = this->DecodeUTF8(&this->data[i], nBytes);
					if (this->EncodeUTF16(codePoint, current))
					{
						current++;
						targetSize += 2;
					}
					current++;
					targetSize += 2;

					i += nBytes;
				}

				this->size = targetSize;
			}

			this->data = this->sharedResource->data;
			this->sharedResource->nElements = this->size;
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
		NOT_IMPLEMENTED_ERROR;
	}

	NOT_IMPLEMENTED_ERROR; //illegal code point
	return 0;
}

bool String::EncodeUTF16(uint32 codePoint, uint16 *pDest) const
{
	if (codePoint < 0x10000)
	{
		*pDest = (uint16)codePoint;
		return false;
	}

	NOT_IMPLEMENTED_ERROR;
	return true;
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

String String::FormatByteSize(uint64 byteSize)
{
	static const char * prefixes[] = {u8"", u8"Ki", u8"Mi", u8"Gi"};
	uint32 i;
	float64 scaled = byteSize;
	for(i = 0; i < sizeof(prefixes)/sizeof(prefixes[0]); i++)
	{
		if(scaled < 1024)
			break;
		scaled /= 1024;
	}
	if(i == sizeof(prefixes)/sizeof(prefixes[0]))
		i--;

	const auto suffix = u8"B";

	String result;
	if(i == 0)
		result = String::Number(byteSize);
	else
		result = String::Number(scaled, 3);

	return result + u8" " + prefixes[i] + suffix;
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
	result.ResizeAdditional(buffer.size);

	auto it = buffer.end();
	for(uint32 i = 0; i < buffer.GetLength(); i++)
	{
		--it;
		result += (*it);
	}

	return result;
}

String String::Number(float64 number, uint8 precision)
{
	char buf[1024];

	sprintf(buf, "%.*g", precision, number);
	//sprintf_s(buf, sizeof(buf), "%.*g", DBL_DIG, value);

	return String::CopyRawString(buf);
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
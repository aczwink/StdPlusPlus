/*
 * Copyright (c) 2017-2022 Amir Czwink (amir130@hotmail.de)
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
#include <clocale>
#include <cstdio>
//Local
#include <Std++/Containers/Strings/ConstStringIterator.hpp>
#include <Std++/Char.hpp>
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;

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
	uint32 cmpLength = Math::Min(this->length, rhs.length);

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
	
	return (--this->end()).EqualsReversed(--string.end(), string.length);
}

uint32 String::Find(const String &string, uint32 startPos, uint32 length) const
{
	startPos = Math::Min(startPos, this->length-1);
	length = Math::Min(length, this->length - startPos);

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
				return Unsigned<uint32>::Max();
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

	return Unsigned<uint32>::Max();
}

uint32 String::FindReverse(const String &string, uint32 startPos, uint32 length) const
{
	startPos = Math::Min(startPos, this->length - 1);
	length = Math::Min(length, startPos + 1);

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
				return Unsigned<uint32>::Max();
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

	return Unsigned<uint32>::Max();
}

uint64 String::ParseHexNumber() const
{
	auto it = this->begin();

	//skip optional +
	if(*it == u8'+')
		++it;

	uint64 result = 0;
	while(it != this->end())
	{
		uint32 charValue;
		if(IsValueInInterval(*it, (uint32)u8'0', (uint32)u8'9'))
			charValue = *it - u8'0';
		else if(IsValueInInterval(StdXX::ToLowercase(*it), (uint32)u8'a', (uint32)u8'z'))
			charValue = StdXX::ToLowercase(*it) - u8'a';
		else
			NOT_IMPLEMENTED_ERROR;

		uint64 pos = this->length - it.GetPosition() - 1;
		result += Power(16_u64, pos) * charValue;

		++it;
	}

	return result;
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

String String::Reversed() const
{
	String tmp;

	if(this->data == nullptr)
		return tmp;

	tmp.sharedResource = new Resource;
	tmp.sharedResource->isUTF8 = this->IsUTF8();
	tmp.sharedResource->EnsureCapacity(this->size);

	auto it = --this->end();
	for(uint64 i = 0; i < this->length; i++)
	{
		tmp.sharedResource->nElements += tmp.Encode(*it, &tmp.sharedResource->data[tmp.sharedResource->nElements]);
		--it;
	}

	tmp.data = tmp.sharedResource->data;
	tmp.length = this->length;
	tmp.size = tmp.sharedResource->nElements;

	return tmp;
}

DynamicArray<String> String::Split(const String &seperator) const
{
	DynamicArray<String> result;

	int32 pos;
	uint32 oldPos = 0;
	while((pos = this->Find(seperator, oldPos)) != Unsigned<uint32>::Max())
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
	length = Math::Min(length, this->length - startPos);

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

int64 String::ToInt() const
{
	if (this->StartsWith(u8"-"))
	{
		uint64 v = this->SubString(1).ToUInt();
		return -int64(v);
	}
	return this->ToUInt();
}

String String::ToLowercase() const
{
	String tmp;

	tmp.sharedResource = new Resource;
	tmp.sharedResource->EnsureCapacity(this->length * 4); //worst case: every code point takes 4 bytes or is a surrogate
	if(tmp.sharedResource->isUTF8)
	{
		for(uint32 codePoint : *this)
			tmp.sharedResource->nElements += this->EncodeUTF8(StdXX::ToLowercase(codePoint), &tmp.sharedResource->data[tmp.sharedResource->nElements]);
	}
	else
	{
		for (uint32 codePoint : *this)
			tmp.sharedResource->nElements += 2 + 2 * this->EncodeUTF16(StdXX::ToLowercase(codePoint), (uint16 *)&tmp.sharedResource->data[tmp.sharedResource->nElements]);
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
		if(!IsValueInInterval(*it, (uint32)u8'0', (uint32)u8'9'))
			NOT_IMPLEMENTED_ERROR;

		result += Power((uint64)10, (uint64)(this->length - it.GetPosition() - 1)) * (*it - u8'0');
		++it;
	}

	return result;
}

String String::ToUppercase() const
{
	String tmp;

	tmp.sharedResource = new Resource;
	tmp.sharedResource->EnsureCapacity(this->length * 4); //worst case: every code point takes 4 bytes or is a surrogate
	if(tmp.sharedResource->isUTF8)
	{
		for(uint32 codePoint : *this)
			tmp.sharedResource->nElements += this->EncodeUTF8(StdXX::ToUppercase(codePoint), &tmp.sharedResource->data[tmp.sharedResource->nElements]);
	}
	else
	{
		for (uint32 codePoint : *this)
			tmp.sharedResource->nElements += 2 + 2 * this->EncodeUTF16(StdXX::ToUppercase(codePoint), (uint16 *)&tmp.sharedResource->data[tmp.sharedResource->nElements]);
	}
	tmp.data = tmp.sharedResource->data;
	tmp.length = this->length;
	tmp.size = tmp.sharedResource->nElements;

	return tmp;
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

String String::Trim() const
{
	auto it = this->begin();
	auto it2 = this->end();

	//trim from beginning
	while(IsWhiteSpaceChar(*it))
		++it;

	//trim from end
	--it2;
	while(IsWhiteSpaceChar(*it2))
		--it2;
	++it2;

	String result = *this;
	result.length = it2.position - it.position;
	result.size = static_cast<uint32>(it2.current - it.current);
	result.data = it.current;

	return result;
}

//Private methods
uint32 String::DecodeUTF8(const uint8 *src, uint8 &nBytes) const
{
	uint8 b1 = *src++;
	if(b1 & 0x80_u8)
	{
		ASSERT(b1 & 0x40, u8"Bit 6 must be set if bit 7 is also");

		//2 bytes or more...
		uint8 b2 = *src++;
		ASSERT((b2 & 0xC0_u8) == 0x80_u8, u8"Illegal encoding.");

		if(b1 & 0x20_u8)
		{
			//3 or 4 bytes...
			uint8 b3 = *src++;
			ASSERT((b3 & 0xC0_u8) == 0x80_u8, u8"Illegal encoding.");

			if(b1 & 0x10_u8)
			{
				uint8 b4 = *src++;
				ASSERT((b4 & 0xC0_u8) == 0x80_u8, u8"Illegal encoding.");

				nBytes = 4;
				return (uint32(b1 & 0x7_u8) << 18_u32) |
					(uint32(b2 & 0x3F_u8) << 12_u32) |
					(uint32(b3 & 0x3F_u8) << 6_u32) |
					(b4 & 0x3F_u8);
			}

			//3 byte
			nBytes = 3;
			return (uint32(b1 & 0xF_u8) << 12_u32) | (uint32(b2 & 0x3F_u8) << 6_u32) | (b3 & 0x3F_u8);
		}

		//2 byte
		nBytes = 2;
		return (uint32(b1 & 0x1F_u8) << 6_u32) | (b2 & 0x3F_u8);
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
		}
		else
			this->sharedResource->EnsureCapacity(this->size);

		this->data = this->sharedResource->data;
		this->sharedResource->nElements = this->size;
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

	if(codePoint <= UNICODE_MAX)
	{
		//4 bytes
		*dest++ = static_cast<byte>(0xF0 | (codePoint >> 18));
		*dest++ = static_cast<byte>(0x80 | ((codePoint >> 12) & 0x3F));
		*dest++ = static_cast<byte>(0x80 | ((codePoint >> 6) & 0x3F));
		*dest++ = static_cast<byte>(0x80 | (codePoint & 0x3F));

		return 4;
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

String String::CopyRawString(const uint16 *utf16, uint32 nChars)
{
	String str;

	str.sharedResource = new Resource;
	str.sharedResource->isUTF8 = false;

	if (nChars == Unsigned<uint32>::Max())
		str.length = str.CountUTF16Length(utf16, str.size);
	else
	{
		str.length = nChars;
		str.size = str.CountUTF16Size(utf16, nChars);
	}
	str.ResizeAdditional(str.size);

	MemCopy(str.sharedResource->data, utf16, str.size);
	str.sharedResource->nElements = str.size;

	return str;
}

String String::CopyUtf8Bytes(const uint8 *utf8, uint32 size)
{
	String str;

	str.sharedResource = new Resource;
	str.sharedResource->isUTF8 = true;

	str.ResizeAdditional(size);
	str.length = 0;

	uint8* dest = str.sharedResource->data;
	for(uint32 i = 0; i < size;)
	{
		uint8 nBytes;
		uint32 codePoint = str.DecodeUTF8(utf8, nBytes);
		str.EncodeUTF8(codePoint, dest);
		str.length++;

		utf8 += nBytes;
		dest += nBytes;
		i += nBytes;

		ASSERT(i <= size, u8"Invalid encoded data.");
	}
	str.size = size;
	str.sharedResource->nElements = str.size;

	return str;
}

String String::FormatBinaryPrefixed(uint64 byteSize, const String &suffix)
{
	static const char8_t* prefixes[] = {u8"", u8"Ki", u8"Mi", u8"Gi"};
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

	String result;
	if(i == 0)
		result = String::Number(byteSize);
	else
		result = String::Number(scaled, FloatDisplayMode::FixedPointNotation, 2);

	return result + u8" " + prefixes[i] + suffix;
}

String String::HexNumber(uint64 value, uint8 nMinChars, bool addBase)
{
	if (addBase)
		return u8"0x" + String::Number(value, 16, nMinChars);
	return String::Number(value, 16, nMinChars);
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

String String::Number(float64 number, FloatDisplayMode displayMode, uint8 precision)
{
	char buf[1024];

	String format = u8"%.*";
	format += (char)displayMode;

	char *currentLocale = setlocale(LC_NUMERIC, "C");
	sprintf(buf, reinterpret_cast<const char *>(format.ToUTF8().GetRawZeroTerminatedData()), precision, number);
	setlocale(LC_NUMERIC, currentLocale);

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
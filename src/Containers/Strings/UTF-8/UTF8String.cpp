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
#include <Std++/Containers/Strings/UTF-8/UTF8String.hpp>
//Local
#include <Std++/Containers/Strings/ByteString.hpp>
#include <Std++/Containers/Strings/StringUtil.h>
#include <Std++/Containers/Strings/UTF-16/UTF16String.hpp>
#include <Std++/Containers/Strings/UTF32String.hpp>
//Namespaces
using namespace StdXX;

//Operators
UTF8String &UTF8String::operator=(const UTF8String &refString)
{
    this->EnsureCapacity(refString.nElements);

    this->length = refString.length;
    this->nElements = refString.nElements;

    MemCopy(this->data, refString.data, this->GetSize());
    this->data[this->GetSize()] = 0;

    return *this;
}

UTF8String &UTF8String::operator=(UTF8String &&refString)
{
    this->Release();

    this->capacity = refString.capacity;
    this->elementsAllocInterval = refString.elementsAllocInterval;
    this->length = refString.length;
    this->nElements = refString.nElements;
    this->data = refString.data;

    refString.data = nullptr;

    return *this;
}

UTF8String &UTF8String::operator=(const char *string)
{
	this->nElements = GetStringLength(string);
    this->EnsureCapacity(this->nElements);

	//copy data
	MemCopy(this->data, string, this->nElements);
	this->data[this->GetSize()] = 0;

	//compute length
	this->length = 0;
    while(*string)
    {
		uint8 codePointSize;
		this->Decode((const byte *)string, codePointSize);

		this->length++;
		string += codePointSize;
    }

    return *this;
}

UTF8String &UTF8String::operator=(const uint16 *pString)
{
    uint32 i;

    for(i = 0; true; i++)
    {
        if(pString[i] == 0)
            break;
    }

    this->length = i;
    this->EnsureAdditionalCapacity(3 * this->length);

    this->nElements = 0;
    for(i = 0; i < this->length; i++)
    {
        this->nElements += this->Encode(pString[i], &this->data[this->nElements]);
    }

    this->data[this->GetSize()] = 0;

    return *this;
}

UTF8String &UTF8String::operator=(const ByteString &refString)
{
    //each char can be encoded to one or two bytes
    this->EnsureCapacity(2 * refString.GetLength());

    this->nElements = 0;
    for(uint32 i = 0; i < refString.GetLength(); i++)
    {
        this->nElements += this->Encode(refString[i], &this->data[this->nElements]);
    }

    this->length = refString.GetLength();
    this->data[this->GetSize()] = 0;

    return *this;
}

UTF8String &UTF8String::operator=(const UTF16String &refString)
{
    //worst-case: all need to be encoded into 4 bytes
    this->EnsureCapacity(4 * refString.GetLength());

    this->nElements = 0;
    for(uint32 codePoint : refString)
    {
        this->nElements += this->Encode(codePoint, &this->data[this->nElements]);
    }

    this->length = refString.GetLength();
    this->data[this->nElements] = 0;

    return *this;
}

UTF8String &UTF8String::operator=(const UTF32String &refString)
{
    //worst-case: all need to be encoded into 4 bytes
    this->EnsureCapacity(4 * refString.GetLength());

    this->nElements = 0;
    for(uint32 i = 0; i < refString.GetLength(); i++)
    {
        this->nElements += this->Encode(refString[i], &this->data[this->nElements]);
    }

    this->length = refString.GetLength();
    this->data[this->nElements] = 0;

    return *this;
}

UTF8String &UTF8String::operator += (uint32 codePoint)
{
    this->EnsureAdditionalCapacity(4); //worst-case, we need 4 bytes to store the codepoint

    this->nElements += this->Encode(codePoint, &this->data[this->nElements]);
    this->length++;
    this->data[this->nElements] = 0;

    return *this;
}

UTF8String &UTF8String::operator+=(const UTF8String &refString)
{
    this->EnsureAdditionalCapacity(refString.GetNumberOfElements());

    MemCopy(&this->data[this->nElements], refString.GetC_Str(), refString.GetNumberOfElements());
    this->nElements += refString.GetNumberOfElements();
    this->length += refString.GetLength();
    this->data[this->nElements] = 0;

    return *this;
}

//Private methods
uint32 UTF8String::Decode(const byte *pSrc, uint8 &refNBytes) const
{
    uint32 b1;

    b1 = *pSrc++;

    if(b1 & 0x80)
    {
        if(b1 & 0x20)
        {
            //3 or 4 bytes
            NOT_IMPLEMENTED_ERROR;
        }

        refNBytes = 2;

        return ((b1 & 0x1F) << 6) | (*pSrc & 0x3F);
    }

    refNBytes = 1;

    return b1;
}

uint8 UTF8String::Encode(uint32 codePoint, byte *pDest) const
{
    if(codePoint <= 0x7F)
    {
        //ascii -> map directly
        *pDest = (codePoint & 0xFF);
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
        *pDest++ = (0xC0 | (codePoint >> 6));

        /*
        second byte:
        7 -> 1
        6 -> 0
        5...0 -> bits 5 to 0 of codepoint
        */
        *pDest++ = (0x80 | (codePoint & 0x3F));

        return 2;
    }

    if(codePoint <= 0xFFFF)
    {
        //3 bytes
        *pDest++ = 0xE0 | (codePoint >> 12);
        *pDest++ = 0x80 | ((codePoint >> 6) & 0x3F);
        *pDest++ = 0x80 | (codePoint & 0x3F);

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
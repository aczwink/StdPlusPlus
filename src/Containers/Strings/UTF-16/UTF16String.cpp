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
#include <ACStdLib/Containers/Strings/UTF-16/UTF16String.hpp>
//Local
#include <ACStdLib/Containers/Strings/StringUtil.h>
#include <ACStdLib/Containers/Strings/UTF32String.hpp>
#include <ACStdLib/Containers/Strings/UTF-8/UTF8String.hpp>
#include <ACStdLib/Char.hpp>
//Namespaces
using namespace ACStdLib;

//Constructors
UTF16String::UTF16String(const uint16 *pString, uint32 length)
{
    this->nElements = length;
    this->length = length;

    this->EnsureCapacity(length);

    for(uint32 i = 0; i < length; i++)
    {
        this->data[i] = pString[i];
    }
    this->data[this->nElements] = 0;
}

//Operators
UTF16String &UTF16String::operator=(const UTF16String &refString) //copy assign
{
    this->EnsureCapacity(refString.GetCapacity());

    MemCopy(this->data, refString.data, refString.GetSize());
    this->nElements = refString.nElements;
    this->length = refString.length;
    this->data[this->nElements] = 0;

    return *this;
}

UTF16String &UTF16String::operator=(UTF16String &&refString) //move assign
{
    ASSERT(0);
    return *this;
}

UTF16String &UTF16String::operator=(const uint16 *pString)
{
    this->length = GetStringLength(pString);
    this->nElements = this->length;

    this->EnsureCapacity(this->length); //for sure no surrogates needed

    for(uint32 i = 0; i < this->length; i++)
        this->data[i] = pString[i];

    this->data[this->nElements] = 0;

    return *this;
}

UTF16String &UTF16String::operator=(const ByteString &refString)
{
    //each byte can be encoded in one element, can't have surrogates
    this->EnsureCapacity(refString.GetLength());

    for(uint32 i = 0; i < refString.GetLength(); i++)
    {
        this->data[i] = refString[i];
    }
    this->nElements = refString.GetLength();
    this->length = this->nElements;

    this->data[this->nElements] = 0;

    return *this;
}

UTF16String &UTF16String::operator=(const UTF8String &refString)
{
    //worst-case: everything will be a surrogate
    this->EnsureCapacity(2 * refString.GetLength());

    this->nElements = 0;
    for(uint32 codePoint : refString)
    {
        if(this->Encode(codePoint, &this->data[this->nElements]))
            this->nElements++;

        this->nElements++;
        this->length++;
    }
    this->data[this->nElements] = 0;

    return *this;
}

UTF16String &UTF16String::operator=(const UTF32String &refString)
{
    //worst-case: everything will be a surrogate
    this->EnsureCapacity(2 * refString.GetLength());

    this->nElements = 0;
    for(uint32 i = 0; i < refString.GetLength(); i++)
    {
        if(this->Encode(refString[i], &this->data[this->nElements]))
            this->nElements++;

        this->nElements++;
        this->length++;
    }
    this->data[this->nElements] = 0;

    return *this;
}

UTF16String UTF16String::operator+(const UTF16String &refRight) const
{
    UTF16String tmp;

    tmp.EnsureCapacity(this->GetLength() + refRight.GetLength());
    tmp += *this;
    tmp += refRight;

    return tmp;
}

UTF16String &UTF16String::operator+=(uint16 c)
{
    //can't have surrogates
    this->EnsureAdditionalCapacity(1);

    this->data[this->nElements++] = c;
    this->length++;
    this->data[this->nElements] = 0;

    return *this;
}

UTF16String &UTF16String::operator += (const ByteString &refString)
{
    //each byte can be encoded in one element, can't have surrogates
    this->EnsureAdditionalCapacity(refString.GetLength());

    for(uint32 i = 0; i < refString.GetLength(); i++)
    {
        this->data[this->nElements + i] = refString[i];
    }
    this->nElements += refString.GetLength();
    this->length += refString.GetLength();

    this->data[this->nElements] = 0;

    return *this;
}

UTF16String &UTF16String::operator+=(const UTF16String &refString)
{
    this->EnsureAdditionalCapacity(refString.GetCapacity());

    MemCopy(&this->data[this->nElements], refString.data, refString.GetSize());

    this->nElements += refString.GetNumberOfElements();
    this->length += refString.GetLength();

    this->data[this->nElements] = 0;

    return *this;
}

bool UTF16String::operator<(const UTF16String &refRight) const
{
    int32 cmp;

    if(this->GetLength() < refRight.GetLength())
    {
        cmp = MemCmp(this->GetC_Str(), refRight.GetC_Str(), this->GetSize());

        return cmp ? cmp < 0 : true;
    }
    else if(this->GetLength() > refRight.GetLength())
    {
        cmp = MemCmp(this->GetC_Str(), refRight.GetC_Str(), refRight.GetSize());

        return cmp ? cmp < 0 : false;
    }

    return MemCmp(this->GetC_Str(), refRight.GetC_Str(), this->GetSize()) < 0;
}

bool UTF16String::operator>(const UTF16String &refRight) const
{
    int32 cmp;

    if(this->GetLength() < refRight.GetLength())
    {
        cmp = MemCmp(this->GetC_Str(), refRight.GetC_Str(), this->GetSize());

        return cmp ? cmp > 0 : false;
    }
    else if(this->GetLength() > refRight.GetLength())
    {
        cmp = MemCmp(this->GetC_Str(), refRight.GetC_Str(), refRight.GetSize());

        return cmp ? cmp > 0 : true;
    }

    return MemCmp(this->GetC_Str(), refRight.GetC_Str(), this->GetSize()) > 0;
}

//Private methods
uint32 UTF16String::Decode(const uint16 *pSrc, bool &refIsSurrogate) const
{
    if(*pSrc > 0xD800)
    {
        ASSERT(0);
    }

    refIsSurrogate = false;
    return *pSrc;
}

bool UTF16String::Encode(uint32 codePoint, uint16 *pDest)
{
    if(codePoint < 0x10000)
    {
        *pDest = (uint16)codePoint;
        return false;
    }

    ASSERT(0);
    return true;
}

//Public methods
bool UTF16String::Contains(uint32 codePoint) const
{
    for(uint32 current : *this)
        if(current == codePoint)
            return true;

    return false;
}

uint32 UTF16String::Find(uint16 c, uint32 startPos) const
{
    for(uint32 i = startPos; i < this->GetLength(); i++)
    {
        if(this->data[i] == c)
            return i;
    }

    return Natural<uint32>::Max();
}

uint32 UTF16String::FindReverse(uint16 c, uint32 startPos) const
{
    if((startPos == 0) || (this->GetLength() == 0))
        return Natural<uint32>::Max();

    if(startPos > this->GetLength())
        startPos = this->GetLength() - 1;

    for(int32 i = startPos; i >= 0; i--)
    {
        if(this->data[i] == c)
            return i;
    }

    return Natural<uint32>::Max();
}

UTF16String UTF16String::ToLowercase() const
{
    UTF16String buffer;

    buffer.EnsureCapacity(this->GetLength());
    for(uint32 i = 0; i < this->GetLength(); i++)
    {
        buffer.data[i] = ACStdLib::ToLowercase(this->data[i]);
    }
    buffer.nElements = this->nElements;
    buffer.length = this->length;

    return buffer;
}
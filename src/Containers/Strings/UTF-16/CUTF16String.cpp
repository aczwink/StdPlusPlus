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
#include "../../../../headers/Containers/Strings/UTF-16/CUTF16String.h"
//Local
#include "../../../../headers/Containers/Strings/String.h"
#include "../../../../headers/Containers/Strings/CUTF32String.h"
#include "../../../../headers/Containers/Strings/UTF-8/CUTF8String.h"
//Namespaces
using namespace ACStdLib;

//Constructors
CUTF16String::CUTF16String(const uint16 *pString, uint32 length)
{
    this->nElements = length;
    this->length = length;

    this->EnsureCapacity(length);

    for(uint32 i = 0; i < length; i++)
    {
        this->pData[i] = pString[i];
    }
    this->pData[this->nElements] = 0;
}

//Operators
CUTF16String &CUTF16String::operator=(const CUTF16String &refString) //copy assign
{
    this->EnsureCapacity(refString.GetCapacity());

    MemCopy(this->pData, refString.pData, refString.GetSize());
    this->nElements = refString.nElements;
    this->length = refString.length;
    this->pData[this->nElements] = 0;

    return *this;
}

CUTF16String &CUTF16String::operator=(CUTF16String &&refString) //move assign
{
    ASSERT(0);
    return *this;
}

CUTF16String &CUTF16String::operator=(const char *pStr)
{
    this->nElements = 0;
    this->length = GetStringLength(pStr);

    this->EnsureCapacity(this->length); //for sure no surrogates needed

    for(uint32 i = 0; i < this->length; i++)
        this->pData[this->nElements++] = pStr[i];

    this->pData[this->nElements] = 0;

    return *this;
}

CUTF16String &CUTF16String::operator=(const uint16 *pString)
{
    this->length = GetStringLength(pString);
    this->nElements = this->length;

    this->EnsureCapacity(this->length); //for sure no surrogates needed

    for(uint32 i = 0; i < this->length; i++)
        this->pData[i] = pString[i];

    this->pData[this->nElements] = 0;

    return *this;
}

CUTF16String &CUTF16String::operator=(const C8BitString &refString)
{
    //each byte can be encoded in one element, can't have surrogates
    this->EnsureCapacity(refString.GetLength());

    for(uint32 i = 0; i < refString.GetLength(); i++)
    {
        this->pData[i] = refString[i];
    }
    this->nElements = refString.GetLength();
    this->length = this->nElements;

    this->pData[this->nElements] = 0;

    return *this;
}

CUTF16String &CUTF16String::operator=(const CUTF8String &refString)
{
    //worst-case: everything will be a surrogate
    this->EnsureCapacity(2 * refString.GetLength());

    this->nElements = 0;
    for(uint32 codePoint : refString)
    {
        if(this->Encode(codePoint, &this->pData[this->nElements]))
            this->nElements++;

        this->nElements++;
        this->length++;
    }
    this->pData[this->nElements] = 0;

    return *this;
}

CUTF16String &CUTF16String::operator=(const CUTF32String &refString)
{
    //worst-case: everything will be a surrogate
    this->EnsureCapacity(2 * refString.GetLength());

    this->nElements = 0;
    for(uint32 i = 0; i < refString.GetLength(); i++)
    {
        if(this->Encode(refString[i], &this->pData[this->nElements]))
            this->nElements++;

        this->nElements++;
        this->length++;
    }
    this->pData[this->nElements] = 0;

    return *this;
}

CUTF16String CUTF16String::operator+(const CUTF16String &refRight) const
{
    CUTF16String tmp;

    tmp.EnsureCapacity(this->GetLength() + refRight.GetLength());
    tmp += *this;
    tmp += refRight;

    return tmp;
}

CUTF16String &CUTF16String::operator+=(uint16 c)
{
    //can't have surrogates
    this->EnsureAdditionalCapacity(1);

    this->pData[this->nElements++] = c;
    this->length++;
    this->pData[this->nElements] = 0;

    return *this;
}

CUTF16String &CUTF16String::operator += (const C8BitString &refString)
{
    //each byte can be encoded in one element, can't have surrogates
    this->EnsureAdditionalCapacity(refString.GetLength());

    for(uint32 i = 0; i < refString.GetLength(); i++)
    {
        this->pData[this->nElements + i] = refString[i];
    }
    this->nElements += refString.GetLength();
    this->length += refString.GetLength();

    this->pData[this->nElements] = 0;

    return *this;
}

CUTF16String &CUTF16String::operator+=(const CUTF16String &refString)
{
    this->EnsureAdditionalCapacity(refString.GetCapacity());

    MemCopy(&this->pData[this->nElements], refString.pData, refString.GetSize());

    this->nElements += refString.GetNumberOfElements();
    this->length += refString.GetLength();

    this->pData[this->nElements] = 0;

    return *this;
}

bool CUTF16String::operator<(const CUTF16String &refRight) const
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

bool CUTF16String::operator>(const CUTF16String &refRight) const
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
uint32 CUTF16String::Decode(const uint16 *pSrc, bool &refIsSurrogate) const
{
    if(*pSrc > 0xD800)
    {
        ASSERT(0);
    }

    refIsSurrogate = false;
    return *pSrc;
}

bool CUTF16String::Encode(uint32 codePoint, uint16 *pDest)
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
bool CUTF16String::Contains(uint32 codePoint) const
{
    for(uint32 current : *this)
        if(current == codePoint)
            return true;

    return false;
}

uint32 CUTF16String::Find(uint16 c, uint32 startPos) const
{
    for(uint32 i = startPos; i < this->GetLength(); i++)
    {
        if(this->pData[i] == c)
            return i;
    }

    return UINT32_MAX;
}

uint32 CUTF16String::FindReverse(uint16 c, uint32 startPos) const
{
    if((startPos == 0) || (this->GetLength() == 0))
        return UINT32_MAX;

    if(startPos > this->GetLength())
        startPos = this->GetLength() - 1;

    for(int32 i = startPos; i >= 0; i--)
    {
        if(this->pData[i] == c)
            return i;
    }

    return UINT32_MAX;
}

CUTF16String CUTF16String::ToLowercase() const
{
    CUTF16String buffer;

    buffer.EnsureCapacity(this->GetLength());
    for(uint32 i = 0; i < this->GetLength(); i++)
    {
        buffer.pData[i] = ACStdLib::ToLowercase(this->pData[i]);
    }
    buffer.nElements = this->nElements;
    buffer.length = this->length;

    return buffer;
}
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
#include <ACStdLib/Containers/Strings/C8BitString.h>
//Global
#include <ctype.h>
#include <string.h>
//Local
#include <ACStdLib/Definitions.h>
#include <ACStdLib/Memory.h>
//Namespaces
using namespace ACStdLib;

//Operators
C8BitString &C8BitString::operator=(const char *pStr)
{
    AFixedCharLengthString<char>::Assign(pStr, (uint32)strlen(pStr));

    return *this;
}

C8BitString &C8BitString::operator=(C8BitString &&refString)
{
    this->Release();

    this->capacity = refString.capacity;
    this->elementsAllocInterval = refString.elementsAllocInterval;
    this->nElements = refString.nElements;
    this->pData = refString.pData;

    refString.pData = nullptr;

    return *this;
}

C8BitString &C8BitString::operator+=(const C8BitString &refString)
{
    this->EnsureAdditionalCapacity(refString.GetLength());

    MemCopy(this->pData + this->GetLength(), refString.pData, refString.GetLength());
    this->nElements += refString.nElements;
    this->pData[this->GetLength()] = 0;

    return *this;
}

//Public methods
int32 C8BitString::Find(const C8BitString &refSearch, uint32 startPos) const
{
    int32 pos = startPos;
    char *ptr;

    if(refSearch.GetLength() == 0)
        return startPos;

    while((pos = this->Find(refSearch[0], pos)) != UINT32_MAX)
    {
        ptr = this->pData + pos;

        if(MemCmp(refSearch.pData, ptr, refSearch.GetLength()) == 0)
            return pos;
        else
            pos++;
    }

    return UINT32_MAX;
}

int32 C8BitString::FindReverse(const C8BitString &refSearch, uint32 startPos) const
{
    int32 pos = startPos;

    if(refSearch.GetLength() == 0)
        return startPos;

    while((pos = this->FindReverse((char)refSearch[0], pos)) != UINT32_MAX)
    {
        if(this->SubString(pos, refSearch.GetLength()) == refSearch)
            return pos;
        pos--;
    }

    return UINT32_MAX;
}

void C8BitString::Replace(const C8BitString &refSearch, const C8BitString &refReplace)
{
    C8BitString buffer;
    int32 pos = 0, oldPos = 0;

    if(refSearch.IsEmpty())
        return;

    while((pos = this->Find(refSearch, pos)) != UINT32_MAX)
    {
        buffer += this->SubString(oldPos, pos - oldPos) + refReplace;
        pos += refSearch.GetLength();
        oldPos = pos;
    }
    buffer += this->SubString(oldPos, this->GetLength() - oldPos);

    *this = buffer;
}

void C8BitString::Resize(uint32 newLength)
{
    if(newLength > this->nElements)
    {
        this->EnsureCapacity(newLength);
        MemZero(this->pData + this->nElements, newLength - this->nElements);
        this->nElements = newLength;
    }
    else
    {
        this->pData[this->nElements] = 0;
    }
}

CLinkedList<C8BitString> C8BitString::Split(const C8BitString &refDelimiter) const
{
    CLinkedList<C8BitString> result;
    int32 pos, oldPos = 0;

    while((pos = this->Find(refDelimiter, oldPos)) != UINT32_MAX)
    {
        result.InsertTail(this->SubString(oldPos, pos - oldPos));
        oldPos = pos + refDelimiter.GetLength();
    }

    if(this->GetLength() - oldPos != 0)
    {
        result.InsertTail(this->SubString(oldPos, this->GetLength() - oldPos));
    }

    return result;
}

C8BitString C8BitString::SubString(uint32 beginOffset, uint32 length) const
{
    C8BitString buffer;

    if(length == 0)
        return C8BitString();

    buffer.EnsureCapacity(length);

    MemCopy(buffer.pData, this->pData + beginOffset, length);
    buffer.nElements = length;
    buffer[length] = '\0';

    return buffer;
}

C8BitString C8BitString::ToLowercase() const
{
    C8BitString buffer;

    buffer.EnsureCapacity(this->GetLength());
    for(uint32 i = 0; i < this->GetLength(); i++)
    {
        buffer.pData[i] = tolower(this->pData[i]);
    }
    buffer.nElements = this->nElements;

    return buffer;
}

C8BitString C8BitString::ToUppercase() const
{
    C8BitString buffer;

    buffer.EnsureCapacity(this->GetLength());
    for(uint32 i = 0; i < this->GetLength(); i++)
    {
        buffer.pData[i] = toupper(this->pData[i]);
    }
    buffer.nElements = this->nElements;

    return buffer;
}

//Outside class operators
C8BitString ACStdLib::operator+(const C8BitString &left, const C8BitString &right)
{
    C8BitString buffer(left);

    buffer += right;

    return buffer;
}

bool ACStdLib::operator==(const C8BitString &left, const C8BitString &right)
{
    if(left.GetLength() != right.GetLength())
        return false;
    return MemCmp(left.GetC_Str(), right.GetC_Str(), left.GetLength()) == 0;
}

bool ACStdLib::operator!=(const C8BitString &left, const C8BitString &right)
{
    if(left.GetLength() != right.GetLength())
        return true;
    return MemCmp(left.GetC_Str(), right.GetC_Str(), left.GetLength()) != 0;
}

bool ACStdLib::operator<(const C8BitString &left, const C8BitString &right)
{
    int32 cmp;

    if(left.GetLength() < right.GetLength())
    {
        cmp = MemCmp(left.GetC_Str(), right.GetC_Str(), left.GetLength());

        return cmp ? cmp < 0 : true;
    }
    else if(left.GetLength() > right.GetLength())
    {
        cmp = MemCmp(left.GetC_Str(), right.GetC_Str(), right.GetLength());

        return cmp ? cmp < 0 : false;
    }

    return MemCmp(left.GetC_Str(), right.GetC_Str(), left.GetLength()) < 0;
}

bool ACStdLib::operator>(const C8BitString &left, const C8BitString &right)
{
    int32 cmp;

    if(left.GetLength() < right.GetLength())
    {
        cmp = MemCmp(left.GetC_Str(), right.GetC_Str(), left.GetLength());

        return cmp ? cmp > 0 : false;
    }
    else if(left.GetLength() > right.GetLength())
    {
        cmp = MemCmp(left.GetC_Str(), right.GetC_Str(), right.GetLength());

        return cmp ? cmp > 0 : true;
    }

    return MemCmp(left.GetC_Str(), right.GetC_Str(), left.GetLength()) > 0;
}
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
#include <ACStdLib/Containers/Strings/ByteString.hpp>
//Global
#include <ctype.h>
#include <string.h>
//Local
#include <ACStdLib/Definitions.h>
#include <ACStdLib/Memory.h>
//Namespaces
using namespace ACStdLib;

//Operators
ByteString &ByteString::operator=(const char *pStr)
{
    FixedCharLengthString<char>::Assign(pStr, (uint32)strlen(pStr));

    return *this;
}

ByteString &ByteString::operator=(ByteString &&refString)
{
    this->Release();

    this->capacity = refString.capacity;
    this->elementsAllocInterval = refString.elementsAllocInterval;
    this->nElements = refString.nElements;
    this->data = refString.data;

    refString.data = nullptr;

    return *this;
}

ByteString &ByteString::operator+=(const ByteString &refString)
{
    this->EnsureAdditionalCapacity(refString.GetLength());

    MemCopy(this->data + this->GetLength(), refString.data, refString.GetLength());
    this->nElements += refString.nElements;
    this->data[this->GetLength()] = 0;

    return *this;
}

//Public methods
int32 ByteString::Find(const ByteString &refSearch, uint32 startPos) const
{
    int32 pos = startPos;
    char *ptr;

    if(refSearch.GetLength() == 0)
        return startPos;

    while((pos = this->Find(refSearch[0], pos)) != UINT32_MAX)
    {
        ptr = this->data + pos;

        if(MemCmp(refSearch.data, ptr, refSearch.GetLength()) == 0)
            return pos;
        else
            pos++;
    }

    return UINT32_MAX;
}

int32 ByteString::FindReverse(const ByteString &refSearch, uint32 startPos) const
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

void ByteString::Replace(const ByteString &refSearch, const ByteString &refReplace)
{
    ByteString buffer;
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

void ByteString::Resize(uint32 newLength)
{
    if(newLength > this->nElements)
    {
        this->EnsureCapacity(newLength);
        MemZero(this->data + this->nElements, newLength - this->nElements);
        this->nElements = newLength;
    }
    else
    {
        this->data[this->nElements] = 0;
    }
}

LinkedList<ByteString> ByteString::Split(const ByteString &refDelimiter) const
{
    LinkedList<ByteString> result;
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

ByteString ByteString::SubString(uint32 beginOffset, uint32 length) const
{
    ByteString buffer;

    if(length == 0)
        return ByteString();

    buffer.EnsureCapacity(length);

    MemCopy(buffer.data, this->data + beginOffset, length);
    buffer.nElements = length;
    buffer[length] = '\0';

    return buffer;
}

ByteString ByteString::ToLowercase() const
{
    ByteString buffer;

    buffer.EnsureCapacity(this->GetLength());
    for(uint32 i = 0; i < this->GetLength(); i++)
    {
        buffer.data[i] = tolower(this->data[i]);
    }
    buffer.nElements = this->nElements;

    return buffer;
}

ByteString ByteString::ToUppercase() const
{
    ByteString buffer;

    buffer.EnsureCapacity(this->GetLength());
    for(uint32 i = 0; i < this->GetLength(); i++)
    {
        buffer.data[i] = toupper(this->data[i]);
    }
    buffer.nElements = this->nElements;

    return buffer;
}

//Outside class operators
ByteString ACStdLib::operator+(const ByteString &left, const ByteString &right)
{
    ByteString buffer(left);

    buffer += right;

    return buffer;
}

bool ACStdLib::operator==(const ByteString &left, const ByteString &right)
{
    if(left.GetLength() != right.GetLength())
        return false;
    return MemCmp(left.GetC_Str(), right.GetC_Str(), left.GetLength()) == 0;
}

bool ACStdLib::operator!=(const ByteString &left, const ByteString &right)
{
    if(left.GetLength() != right.GetLength())
        return true;
    return MemCmp(left.GetC_Str(), right.GetC_Str(), left.GetLength()) != 0;
}

bool ACStdLib::operator<(const ByteString &left, const ByteString &right)
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

bool ACStdLib::operator>(const ByteString &left, const ByteString &right)
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
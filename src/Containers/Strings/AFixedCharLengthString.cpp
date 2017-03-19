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
#include "../../../headers/Containers/Strings/AFixedCharLengthString.h"
//Local
#include "../../../headers/Mathematics.h"
//Namespaces
using namespace ACStdLib;
//Definitions
#define SJCLIB_CHAR_ZERO 48
#define SJCLIB_CHAR_NINE 57

//Protected methods
template<typename CharType>
void AFixedCharLengthString<CharType>::Append(CharType c)
{
    this->EnsureAdditionalCapacity(1);

    this->pData[this->nElements++] = c;
    this->pData[this->nElements] = 0;
}

template<typename CharType>
void AFixedCharLengthString<CharType>::Append(const CharType *pStr, uint32 length)
{
    this->EnsureAdditionalCapacity(length);

    MemCopy(this->pData + this->GetLength(), pStr, length * sizeof(CharType));
    this->nElements += length;
    this->pData[this->GetLength()] = 0;
}

template<typename CharType>
void AFixedCharLengthString<CharType>::Assign(CharType c)
{
    this->EnsureCapacity(1);

    this->pData[0] = c;
    this->pData[1] = 0;
    this->nElements = 1;
}

template<typename CharType>
void AFixedCharLengthString<CharType>::Assign(const CharType *pStr, uint32 length)
{
    this->EnsureCapacity(length);

    MemCopy(this->pData, pStr, length * sizeof(CharType));
    this->pData[length] = 0;
    this->nElements = length;
}

//Public methods
template<typename CharType>
uint32 AFixedCharLengthString<CharType>::Find(CharType c, uint32 startPos) const
{
    for(uint32 i = startPos; i < this->GetLength(); i++)
    {
        if(this->pData[i] == c)
            return i;
    }

    return UINT32_MAX;
}

template<typename CharType>
int32 AFixedCharLengthString<CharType>::FindReverse(CharType c, uint32 startPos) const
{
    if((startPos <= 0) || (this->GetLength() == 0))
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

//Instantiate the Baseclass
template class ACStdLib::AFixedCharLengthString<char>;
template class ACStdLib::AFixedCharLengthString<uint32>;
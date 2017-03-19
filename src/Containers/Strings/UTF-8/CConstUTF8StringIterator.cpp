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
#include "../../../../headers/Containers/Strings/UTF-8/CConstUTF8StringIterator.h"
//Local
#include "../../../../headers/Containers/Strings/UTF-8/CUTF8String.h"
//Namespaces
using namespace ACStdLib;

//Constructor
CConstUTF8StringIterator::CConstUTF8StringIterator(const CUTF8String &refString, uint32 index) : refString(refString)
{
    this->pCurrent = refString.GetC_Str() + index;
}

//Operators
CConstUTF8StringIterator &CConstUTF8StringIterator::operator++()
{
    uint8 nBytes;

    this->refString.Decode(this->pCurrent, nBytes);
    this->pCurrent += nBytes;

    return *this;
}

uint32 CConstUTF8StringIterator::operator*() const
{
    uint8 nBytes;

    return this->refString.Decode(this->pCurrent, nBytes);
}
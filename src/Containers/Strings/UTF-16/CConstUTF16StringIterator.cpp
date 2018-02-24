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
#include <Std++/Containers/Strings/UTF-16/CConstUTF16StringIterator.h>
//Local
#include <Std++/Containers/Strings/UTF-16/UTF16String.hpp>
//Namespaces
using namespace StdPlusPlus;

//Constructor
CConstUTF16StringIterator::CConstUTF16StringIterator(const UTF16String &refString, uint32 index) : refString(refString)
{
    this->pCurrent = refString.GetC_Str() + index;
    if(this->pCurrent)
        this->currentCodePoint = this->refString.Decode(this->pCurrent, this->isSurrogate);
}

//Operators
CConstUTF16StringIterator &CConstUTF16StringIterator::operator++()
{
    this->pCurrent++;
    if(this->isSurrogate)
        this->pCurrent++;

    this->currentCodePoint = this->refString.Decode(this->pCurrent, this->isSurrogate);

    return *this;
}
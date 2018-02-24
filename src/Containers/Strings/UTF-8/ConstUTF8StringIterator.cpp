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
#include <Std++/Containers/Strings/UTF-8/ConstUTF8StringIterator.hpp>
//Local
#include <Std++/Containers/Strings/UTF-8/UTF8String.hpp>
//Namespaces
using namespace StdPlusPlus;

//Constructor
ConstUTF8StringIterator::ConstUTF8StringIterator(const UTF8String &refString, uint32 index) : refString(refString)
{
    this->pCurrent = refString.GetC_Str() + index;
}

//Operators
ConstUTF8StringIterator &ConstUTF8StringIterator::operator++()
{
    uint8 nBytes;

    this->refString.Decode(this->pCurrent, nBytes);
    this->pCurrent += nBytes;

    return *this;
}

uint32 ConstUTF8StringIterator::operator*() const
{
    uint8 nBytes;

    return this->refString.Decode(this->pCurrent, nBytes);
}
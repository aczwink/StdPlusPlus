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

#pragma once
//Local
#include "../../../Definitions.h"

namespace StdPlusPlus
{
    //Move declarations
    class UTF16String;

    class CConstUTF16StringIterator
    {
    private:
        //Members
        const UTF16String &refString;
        const uint16 *pCurrent;
        bool isSurrogate;
        uint32 currentCodePoint;
    public:
        //Constructor
        CConstUTF16StringIterator(const UTF16String &refString, uint32 index = 0);

        //Operators
        CConstUTF16StringIterator &operator++(); //Prefix ++

        //Inline Operators
        inline uint32 operator*() const
        {
            return this->currentCodePoint;
        }

        inline bool operator!=(const CConstUTF16StringIterator &refOther) const
        {
            return (&this->refString != &refOther.refString) || (this->pCurrent != refOther.pCurrent);
        }
    };
}
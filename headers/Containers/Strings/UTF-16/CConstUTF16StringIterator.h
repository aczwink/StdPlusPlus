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

#pragma once
//Local
#include "../../../Definitions.h"

namespace ACStdLib
{
    //Forward declarations
    class CUTF16String;

    class CConstUTF16StringIterator
    {
    private:
        //Members
        const CUTF16String &refString;
        const uint16 *pCurrent;
        bool isSurrogate;
        uint32 currentCodePoint;
    public:
        //Constructor
        CConstUTF16StringIterator(const CUTF16String &refString, uint32 index = 0);

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
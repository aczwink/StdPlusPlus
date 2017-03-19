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
#include "AInputStream.h"
#include "../Containers/CArray/CArray.h"

namespace ACStdLib
{
    class ACSTDLIB_API CConcatenatedInputStream : public AInputStream
    {
    private:
        //Members
        uint8 currentStream;
        CArray<AInputStream *> partialStreams;

    public:
        //Constructor
        inline CConcatenatedInputStream(AInputStream &refFirstStream)
        {
            this->currentStream = 0;

            *this << refFirstStream;
        }

        //Operators
        inline CConcatenatedInputStream &operator<<(AInputStream &refStream)
        {
            this->partialStreams.Push(&refStream);

            return *this;
        }

        //Methods
        bool HitEnd() const;
        byte ReadByte();
        uint32 ReadBytes(void *pDestination, uint32 count);
        uint32 Skip(uint32 nBytes);
    };
}
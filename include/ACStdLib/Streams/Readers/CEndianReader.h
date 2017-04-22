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
#include "../AInputStream.h"

namespace ACStdLib
{
    class ACSTDLIB_API CEndianReader
    {
    private:
        //Members
        bool readBigEndian;
        AInputStream &refInput;
    public:
        //Constructor
        inline CEndianReader(bool readBigEndian, AInputStream &refInput) : refInput(refInput)
        {
            this->readBigEndian = readBigEndian;
        }

        //Members
        inline int16 ReadInt16()
        {
            if(this->readBigEndian)
                return this->refInput.ReadInt16BE();

            return this->refInput.ReadInt16LE();
        }

        inline uint16 ReadUInt16()
        {
            if(this->readBigEndian)
                return this->refInput.ReadUInt16BE();

            return this->refInput.ReadUInt16LE();
        }

        inline uint32 ReadUInt32()
        {
            if(this->readBigEndian)
                return this->refInput.ReadUInt32BE();

            return this->refInput.ReadUInt32LE();
        }

        inline void Skip(uint32 nBytes)
        {
            this->refInput.Skip(nBytes);
        }
    };
}
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
#include "ACStdLib/Filesystem/Path.hpp"
#include "ASeekableInputStream.h"

namespace ACStdLib
{
    class ACSTDLIB_API FileInputStream : public ASeekableInputStream
    {
    private:
        //Members
        bool hitEnd;
        union
        {
            void *pFileHandle;
            int fileHandle;
        };
    public:
        //Constructor
        FileInputStream(const Path &refPath);
        //Destructor
        ~FileInputStream();
        //Methods
        uint64 GetCurrentOffset() const;
        uint64 GetRemainingBytes() const;
        uint64 GetSize() const;
        bool HitEnd() const;
        byte ReadByte();
        uint32 ReadBytes(void *pDestination, uint32 count);
        void SetCurrentOffset(uint64 offset);
        uint32 Skip(uint32 nBytes);
    };
}
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
#include "InputStream.hpp"

namespace ACStdLib
{
    class ACSTDLIB_API ASeekableInputStream : public InputStream
    {
    public:
        //Abstract
        virtual uint64 GetCurrentOffset() const = NULL;
        virtual uint64 GetRemainingBytes() const = NULL;
        virtual uint64 GetSize() const = NULL;
        virtual void SetCurrentOffset(uint64 offset) = NULL;

        //Inline
        inline void Rewind(uint32 nBytes)
        {
            this->SetCurrentOffset(this->GetCurrentOffset() - nBytes);
        }
    };
}
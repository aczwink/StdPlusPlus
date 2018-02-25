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
#include "../OutputStream.hpp"
#include "../../Containers/Strings/ByteString.hpp"
#include "../../Containers/Strings/UTF-8/UTF8String.hpp"

namespace StdPlusPlus
{
    class STDPLUSPLUS_API CTextWriter
    {
    private:
        //Members
        OutputStream &refOutput;

    public:
        //Constructor
        inline CTextWriter(OutputStream &refOutput) : refOutput(refOutput)
        {
        }

        //Methods
        void WriteUTF8(uint32 codePoint);
        void WriteUTF8_ZeroTerminated(const UTF8String &refString);
    };
}
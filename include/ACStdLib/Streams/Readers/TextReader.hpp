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
#include "ACStdLib/Streams/InputStream.hpp"
#include "ACStdLib/Containers/Strings/ByteString.hpp"
#include "ACStdLib/Containers/Strings/UTF-8/UTF8String.hpp"
#include "DataReader.hpp"

namespace ACStdLib
{
    class ACSTDLIB_API TextReader
    {
    private:
        //Members
        InputStream &inputStream;
		DataReader dataReader;

		//Methods
		bool IsWhitespace(byte b);
		byte SkipWhitespaces();

    public:
        //Constructor
        inline TextReader(InputStream &inputStream) : inputStream(inputStream), dataReader(true, inputStream)
        {
        }

		//Operators
		TextReader &operator>>(uint32 &i);
		TextReader &operator>>(float32 &f);
		TextReader &operator>>(ByteString &target);

        //Methods
        ByteString ReadASCII(uint32 length);
        ByteString ReadASCII_Line();
        ByteString ReadASCII_ZeroTerminated();
        uint32 ReadUTF8();
        UTF8String ReadUTF8Line();
    };
}
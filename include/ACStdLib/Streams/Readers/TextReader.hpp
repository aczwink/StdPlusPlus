/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include "../TextCodec.hpp"
#include "ACStdLib/Streams/InputStream.hpp"
#include "ACStdLib/Containers/Strings/ByteString.hpp"
#include "ACStdLib/Containers/Strings/UTF-8/UTF8String.hpp"
#include "DataReader.hpp"

namespace ACStdLib
{
    class ACSTDLIB_API TextReader
    {
    public:
        //Constructor
        inline TextReader(InputStream &inputStream, TextCodecType codecType) : inputStream(inputStream), codec(TextCodec::GetCodec(codecType)), dataReader(true, inputStream)
        {
        }

		//Destructor
		inline ~TextReader()
		{
			if(this->codec)
				delete this->codec;
		}

		//Operators
		TextReader &operator>>(uint32 &i);
		TextReader &operator>>(float32 &f);
		TextReader &operator>>(ByteString &target);

        //Methods
		/**
		 * Read 'length' characters and return them as string.
		 *
		 * @param length
		 * @return
		 */
        String ReadString(uint32 length);
        ByteString ReadASCII_Line();
        ByteString ReadASCII_ZeroTerminated();
        uint32 ReadUTF8();
        UTF8String ReadUTF8Line();

	private:
		//Members
		InputStream &inputStream;
		DataReader dataReader;
		TextCodec *codec;

		//Methods
		bool IsWhitespace(byte b);
		byte SkipWhitespaces();
    };
}
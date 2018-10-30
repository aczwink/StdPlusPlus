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
#include "../TextCodec.hpp"
#include "../InputStream.hpp"
#include "../../Containers/Strings/ByteString.hpp"
#include "../../Containers/Strings/UTF-8/UTF8String.hpp"
#include "DataReader.hpp"

namespace StdXX
{
    class STDPLUSPLUS_API TextReader
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
		TextReader &operator>>(String &target);

        //Methods
		/**
		 * Read 'length' characters and return them as string.
		 *
		 * @param length
		 * @return
		 */
        String ReadString(uint32 length);
        ByteString ReadASCII_Line();
		String ReadZeroTerminatedString();

		//Inline
		inline bool IsAtEnd() const
		{
			return this->inputStream.IsAtEnd();
		}

		inline uint32 ReadCodePoint()
		{
			return this->codec->ReadCodePoint(this->inputStream);
		}

	private:
		//Members
		InputStream &inputStream;
		DataReader dataReader;
		TextCodec *codec;

		//Methods
		uint32 SkipWhitespaces();
    };
}
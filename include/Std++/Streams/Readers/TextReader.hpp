/*
 * Copyright (c) 2017-2021 Amir Czwink (amir130@hotmail.de)
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
		 * Reads characters until it finds the newline character '\n'.
		 * The newline character is not part of the result.
		 * If the carriage return is found directly before the newline character, it is omitted also.
		 *
		 * @return
		 */
		String ReadLine();
		/**
		 * Read 'length' characters and return them as string.
		 *
		 * @param length
		 * @return
		 */
        String ReadString(uint32 length);
        /**
         * Read characters until 'size' many bytes have been consumed.
         * If 'size' is wrong and more bytes are read then given, an error is raised.
         * @param size
         * @return
         */
        String ReadStringBySize(uint32 size);
		String ReadZeroTerminatedString();
		/**
		 * Read 'length' characters and return them as string.
		 * Zeroes at the end of the resulting string are skipped.
		 *
		 * @param length
		 * @return
		 */
		String ReadZeroTerminatedString(uint32 length);
		String ReadZeroTerminatedStringBySize(uint32 size);

		//Inline
		inline bool IsAtEnd() const
		{
			return this->inputStream.IsAtEnd();
		}

		inline uint32 ReadCodePoint()
		{
			uint8 nBytes;
			return this->codec->ReadCodePoint(this->inputStream, nBytes);
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
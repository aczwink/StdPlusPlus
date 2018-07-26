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
#include <Std++/Streams/TextCodec.hpp>
#include "../OutputStream.hpp"
#include "../../Containers/Strings/ByteString.hpp"
#include "../../Containers/Strings/ConstStringIterator.hpp"
#include "../../Containers/Strings/UTF-8/UTF8String.hpp"

namespace StdXX
{
    class STDPLUSPLUS_API TextWriter
    {
    public:
        //Constructor
        inline TextWriter(OutputStream &outputStream, TextCodecType codecType) : outputStream(outputStream), codec(TextCodec::GetCodec(codecType))
        {
        }

		//Destructor
		inline ~TextWriter()
		{
			if(this->codec)
				delete this->codec;
		}

        //Methods
        void WriteUTF8(uint32 codePoint);
        void WriteUTF8_ZeroTerminated(const UTF8String &refString);

		//Inline
		inline void WriteString(const String &string)
		{
			for(uint32 codePoint : string)
				this->codec->WriteCodePoint(codePoint, this->outputStream);
		}

    private:
        //Members
        OutputStream &outputStream;
		TextCodec *codec;
    };
}
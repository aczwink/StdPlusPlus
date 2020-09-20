/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/OutputStream.hpp>
#include <Std++/Streams/TextCodec.hpp>
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Containers/Strings/ConstStringIterator.hpp>

namespace StdXX
{
	class LineBreak{};
	static const LineBreak endl;

    class STDPLUSPLUS_API TextWriter
    {
    public:
        //Constructor
        inline TextWriter(OutputStream &outputStream, TextCodecType codecType) : outputStream(outputStream), codec(TextCodec::GetCodec(codecType))
        {
        	this->lineSeparator = u8"\r\n";
        }

		//Destructor
		inline ~TextWriter()
		{
			delete this->codec;
		}

	    //Inline Operators
	    inline TextWriter &operator<<(bool b)
	    {
	    	return *this << (b ? u8"true" : u8"false");
	    }

	    inline TextWriter &operator<<(int32 i)
	    {
		    return *this << String::Number(i);
	    }

	    inline TextWriter &operator<<(uint16 i)
	    {
		    return *this << String::Number(i);
	    }

	    inline TextWriter &operator<<(uint32 i)
	    {
		    return *this << String::Number(i);
	    }

	    inline TextWriter &operator<<(uint64 i)
	    {
		    return *this << String::Number(i);
	    }

	    inline TextWriter &operator<<(float64 f)
	    {
		    return *this << String::Number(f);
	    }

	    inline TextWriter &operator<<(const char *string)
	    {
		    *this << String(string);
		    return *this;
	    }

	    inline TextWriter &operator<<(const String &string)
	    {
		    this->WriteString(string);
		    return *this;
	    }

	    inline TextWriter &operator<<(const LineBreak&)
	    {
	    	this->WriteString(this->lineSeparator);
		    return *this;
	    }

		//Inline
		inline void SetLineSeparator(const String& lsep)
		{
			this->lineSeparator = lsep;
		}

		inline void WriteCodePoint(uint32 codePoint)
		{
        	this->codec->WriteCodePoint(codePoint, this->outputStream);
		}

		inline void WriteFixedLengthString(const String& string, uint32 length, uint32 fillChar = 0)
		{
        	ASSERT(string.GetLength() <= length, u8"input string is too long");
        	this->WriteString(string);
        	for(uint32 i = 0; i < length - string.GetLength(); i++)
        		this->WriteCodePoint(fillChar);
		}

		inline void WriteLine()
		{
			this->WriteString(this->lineSeparator);
		}

		inline void WriteLine(const String& line)
		{
			this->WriteString(line);
			this->WriteString(this->lineSeparator);
		}

		inline void WriteString(const String &string)
		{
			for(uint32 codePoint : string)
				this->codec->WriteCodePoint(codePoint, this->outputStream);
		}

		inline void WriteStringZeroTerminated(const String& string)
		{
			this->WriteString(string);
			this->codec->WriteCodePoint(0, this->outputStream);
		}

		inline void WriteTabs(uint16 nTabs)
		{
			for(uint16 i = 0; i < nTabs; i++)
				this->codec->WriteCodePoint(u8'\t', this->outputStream);
		}

    private:
        //Members
        OutputStream &outputStream;
		TextCodec *codec;
		String lineSeparator;
    };
}
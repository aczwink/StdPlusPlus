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
#include "PrintWriterStream.hpp"

namespace StdXX
{
    class AutoLineBreaker
    {
    public:
        //Constructor
        inline AutoLineBreaker(OutputStream &outputStream) : textWriter(outputStream, TextCodecType::UTF8)
        {
        }

        //Destructor
        inline ~AutoLineBreaker()
        {
        	this->textWriter << endl;
        }

        //Inline
        inline AutoLineBreaker &operator<<(const String &string)
        {
            this->textWriter << string;
            return *this;
        }

    private:
        //Members
        TextWriter textWriter;
    };

    class StdErr : public PrintWriterStream
    {
    public:
    	//Methods
	    void Flush() override;
        uint32 WriteBytes(const void *pSource, uint32 size) override;
    };

    class StdOut : public PrintWriterStream
    {
    public:
        //Methods
        void Flush() override;
        uint32 WriteBytes(const void *pSource, uint32 size) override;
    };

    //Global Instances
    extern StdErr STDPLUSPLUS_API stdErr;
    extern StdOut STDPLUSPLUS_API stdOut;
}
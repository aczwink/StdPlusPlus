/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include "../Definitions.h"
#include "../Unsigned.hpp"

namespace StdXX
{
    //Move declarations
    class OutputStream;

    class STDPLUSPLUS_API InputStream
    {
    public:
        //Destructor
        virtual ~InputStream(){};

        //Abstract
		/**
		 * Some streams may buffer data in order to counter high i/o latencies.
		 * This method returns the number of bytes that can be read from the buffer without causing a heavy i/o operation.
		 * Unbuffered streams should return 0.
		 * Of course in concurrent scenarios the number of bytes available can change just between a call to GetBytesAvailable
		 * and ReadBytes.
		 * However, InputStreams have to guarantee that "GetBytesAvailable"-bytes can be safely read from the stream.
		 * @return
		 */
		virtual uint32 GetBytesAvailable() const = 0;
		/**
		 * Returns true as soon as this stream knows that no data is left for reading.
		 * In any other case this method returns false
		 * @return
		 */
        virtual bool IsAtEnd() const = 0;
        virtual uint32 ReadBytes(void *destination, uint32 count) = 0;
		/**
		 * Returns the number of skipped bytes.
		 */
        virtual uint32 Skip(uint32 nBytes) = 0;

        //Methods
        /**
         * Returns the number of bytes that were processed
         * @param outputStream
         * @param The maximum size to flush
         * @return
         */
        uint64 FlushTo(OutputStream &outputStream, uint64 size = Unsigned<uint64>::Max());
    };
}
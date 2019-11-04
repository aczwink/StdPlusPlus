/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
//Global
#include <zlib.h>
//Local
#include <Std++/Compression/Decompressor.hpp>

namespace _stdxx_
{
    class ExtZLIBInflater : public StdXX::Decompressor
    {
        static const uint32 c_bufferSize = 16384;
    public:
        //Constructor
        ExtZLIBInflater(InputStream& inputStream);

        //Destructor
        ~ExtZLIBInflater();

        //Methods
        uint32 GetBytesAvailable() const override;
        bool IsAtEnd() const override;
        uint32 ReadBytes(void *destination, uint32 count) override;
        uint32 Skip(uint32 nBytes) override;

    private:
        //Members
        z_stream strm;
        uint8 inBuffer[c_bufferSize];
        uint8* inBufferCurrent;
        uint32 nBytesInInBuffer;
        uint8 outBuffer[c_bufferSize];
        uint8* outBufferCurrent;
        uint32 nBytesInOutBuffer;

        //Methods
        void DecompressNextBlock();
    };
}
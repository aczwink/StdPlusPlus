/*
 * Copyright (c) 2022 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Buffers/FixedSizeBuffer.hpp>
#include <Std++/Streams/BufferInputStream.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
#include "SMFEvent.hpp"

namespace _stdxx_
{
    class Decoder
    {
    public:
        //Constructor
        inline Decoder(StdXX::InputStream& inputStream) : inputStream(inputStream)
        {
        }

        //Inline
        inline void DecodeNextEvent(SMFEvent& event)
        {
            this->lastStatusByte = Decoder::DecodeEvent(this->inputStream, event, this->lastStatusByte);
        }

    private:
        //Variables
        StdXX::InputStream& inputStream;
        uint8 lastStatusByte;

        //Functions
        static uint8 DecodeEvent(StdXX::InputStream& inputStream, SMFEvent& event, uint8 lastStatusByte);
        static uint32 DecodeVariableLengthInteger(StdXX::DataReader& dataReader);
    };
}
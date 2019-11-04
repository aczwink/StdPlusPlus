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
#pragma once
//Local
#include <Std++/Streams/InputStream.hpp>
#include <Std++/Filesystem/Path.hpp>

namespace _stdxx_
{
    class LocalFileHeader
    {
    public:
        //Members
        uint16 generalPurposeBitFlag;
        uint16 compressionMethod;
        uint32 crc32;
        uint32 compressedSize;
        uint32 uncompressedSize;
        StdXX::Path path;

        //Constructor
        LocalFileHeader(StdXX::InputStream& inputStream);

        //Properties
        inline uint32 HeaderSize() const
        {
            return this->headerSize;
        }

    protected:
        //Members
        uint32 headerSize;

        //Constructor
        inline LocalFileHeader(){}

        //Methods
        void ReadExtraFields(uint16 size, StdXX::InputStream& inputStream);
        void ReadPath(uint16 fileNameSize, StdXX::InputStream& inputStream);
    };
}
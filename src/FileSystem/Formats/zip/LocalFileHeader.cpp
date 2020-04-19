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
//Class header
#include "LocalFileHeader.hpp"
//Local
#include <Std++/Streams/BufferedInputStream.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>
#include "Zip.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Constructor
LocalFileHeader::LocalFileHeader(InputStream &inputStream)
{
    BufferedInputStream bufferedInputStream(inputStream);
    DataReader dataReader(false, bufferedInputStream);

    ASSERT(dataReader.ReadUInt32() == zipLocalFileHeaderSignature, u8"REPORT THIS PLEASE!");
    bufferedInputStream.Skip(2);
    this->generalPurposeBitFlag = dataReader.ReadUInt16();
    this->compressionMethod = dataReader.ReadUInt16();
    bufferedInputStream.Skip(2);
    bufferedInputStream.Skip(2);
    this->crc32 = dataReader.ReadUInt32();
    this->compressedSize = dataReader.ReadUInt32();
    this->uncompressedSize = dataReader.ReadUInt32();
    uint16 fileNameSize = dataReader.ReadUInt16();
    uint16 extraFieldSize = dataReader.ReadUInt16();

    this->ReadPath(fileNameSize, bufferedInputStream);
    this->ReadExtraFields(extraFieldSize, bufferedInputStream);

    this->headerSize = 30 + fileNameSize + extraFieldSize;
}

//Private methods
void LocalFileHeader::ReadExtraFields(uint16 size, InputStream &inputStream)
{
    DataReader dataReader(false, inputStream);

    while(size)
    {
        uint16 headerId = dataReader.ReadUInt16();
        uint16 headerSize = dataReader.ReadUInt16();

        size -= 4;
        size -= headerSize;

        switch(headerId)
        {
        	case 0xA: //NTFS
	        {
	        	ASSERT(headerSize >= 4, u8"REPORT THIS PLEASE!");
	        	dataReader.Skip(4); //reserved
	        	headerSize -= 4;
	        	while(headerSize > 4)
		        {
	        		uint16 tag = dataReader.ReadUInt16();
	        		uint16 tagSize = dataReader.ReadUInt16();
	        		headerSize -= 4;

	        		switch(tag)
			        {
			        	case 1:
				        {
				        	ASSERT(tagSize == 24, u8"REPORT THIS PLEASE!");
					        headerSize -= 24;

				        	uint64 mtime = dataReader.ReadUInt64();
					        uint64 atime = dataReader.ReadUInt64();
					        uint64 ctime = dataReader.ReadUInt64();
				        }
				        break;
				        default:
				        	NOT_IMPLEMENTED_ERROR; //TODO: implement me
			        }
		        }
	        	dataReader.Skip(headerSize);
	        }
	        break;
            case 0x5855: //Info-ZIP UNIX
            {
                ASSERT((headerSize == 8) || (headerSize == 12), u8"REPORT THIS PLEASE!");
                uint32 lastAccessTime = dataReader.ReadUInt32(); //UTC
                uint32 lastModificationTime = dataReader.ReadUInt32(); //UTC
                if(headerSize == 12)
                {
                    dataReader.Skip(2); //uid
                    dataReader.Skip(2); //gid
                }
            }
            break;
            default:
                NOT_IMPLEMENTED_ERROR; //TODO: implement me
                /*
                 * inputStream.Skip(headerSize);
                */
        }
    }
}

void LocalFileHeader::ReadPath(uint16 fileNameSize, InputStream& inputStream)
{
    TextCodecType textEncoding = TextCodecType::CP437;
    if(Unsigned<uint16>::IsBitSet(this->generalPurposeBitFlag, 11))
        textEncoding = TextCodecType::UTF8;

    //read file name
    TextReader textReader(inputStream, textEncoding);
    this->path = u8"/" + textReader.ReadStringBySize(fileNameSize);
}
/*
* Copyright (c) 2019-2021 Amir Czwink (amir130@hotmail.de)
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
/*//Class header
#include "CentralDirectoryRecord.hpp"
//Local
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Streams/TextCodec.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>
#include <Std++/FileSystem/Path.hpp>
#include "Zip.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::FileSystem;

//Constructor
CentralDirectoryRecord::CentralDirectoryRecord(InputStream &inputStream)
{
    DataReader dataReader(false, inputStream);

    ASSERT(dataReader.ReadUInt32() == zipCentralFileHeaderSignature, u8"REPORT THIS PLEASE!");
    this->versionMadeBy = dataReader.ReadUInt16();
    dataReader.Skip(2);
    this->generalPurposeBitFlag = dataReader.ReadUInt16();
    this->compressionMethod = dataReader.ReadUInt16();
    dataReader.Skip(2);
    dataReader.Skip(2);
    this->crc32 = dataReader.ReadUInt32();
    this->compressedSize = dataReader.ReadUInt32();
    this->uncompressedSize = dataReader.ReadUInt32();
    uint16 fileNameSize = dataReader.ReadUInt16();
    uint16 extraFieldSize = dataReader.ReadUInt16();
    uint16 commentLength = dataReader.ReadUInt16();
    dataReader.Skip(2);
    dataReader.Skip(2);
    this->externalFileAttributes = dataReader.ReadUInt32();
    this->localFileHeaderOffset = dataReader.ReadUInt32();

    this->ReadPath(fileNameSize, inputStream);
    this->ReadExtraFields(extraFieldSize, inputStream);
    dataReader.Skip(commentLength);

    this->headerSize = 46 + fileNameSize + extraFieldSize + commentLength;
}

NodeType CentralDirectoryRecord::DetermineType() const
{
    switch(this->versionMadeBy >> 8)
    {
    	case 0: //MS-DOS and OS/2 (FAT / VFAT / FAT32 file systems)
		{
			if(this->externalFileAttributes & 0x10)
				return NodeType::Directory;
			return NodeType::File;
		}
		break;
        case 3: //UNIX
        {
            switch(this->externalFileAttributes >> 28) //upper 4 bits of st_mode
            {
                case 0x4:
                    return NodeType::Directory;
                case 0x8:
                    return NodeType::File;
                case 0xA:
                    return NodeType::Link;
            }
        }
        break;
    }
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return NodeType::File;
}
*/
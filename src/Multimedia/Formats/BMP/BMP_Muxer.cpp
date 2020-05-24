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
//Class Header
#include "BMP_Muxer.hpp"
//Local
#include <Std++/Streams/Writers/DataWriter.hpp>
#include "BMP.hpp"
//Definitions
#define BMP_FILEHEADER_SIZE 14

//Public methods
void BMP_Muxer::Finalize()
{
	uint64 currentOffset = this->outputStream.GetCurrentOffset();

	DataWriter dataWriter(false, this->outputStream);

	//Update file size
	this->outputStream.SeekTo(this->startOffset + BMP_HEADER_TYPE_SIZE);
	dataWriter.WriteUInt32((uint32)(currentOffset - this->startOffset));

	//Update image size
	this->outputStream.SeekTo(this->startOffset + BMP_FILEHEADER_SIZE + 20);
	dataWriter.WriteUInt32(this->imageSize);
}

void BMP_Muxer::WriteHeader()
{
	VideoStream *stream;

	stream = (VideoStream *)this->GetStream(0);

	this->startOffset = this->outputStream.GetCurrentOffset();

	DataWriter dataWriter(false, this->outputStream);

	//bitmap file header
	this->outputStream.WriteBytes(BMP_HEADER_TYPE, BMP_HEADER_TYPE_SIZE);
	dataWriter.WriteUInt32(0); //file size... updated in finalize
	dataWriter.WriteUInt16(0); //reserved1
	dataWriter.WriteUInt16(0); //reserved2
	dataWriter.WriteUInt32(BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE); //data offset

	_stdxx_::WriteBitmapInfoHeader(*stream, this->outputStream);
}

void BMP_Muxer::WritePacket(const IPacket& packet)
{
	VideoStream *pStream = (VideoStream *)this->GetStream(0);
	
	switch(pStream->codingParameters.codingFormat->GetId())
	{
	case CodingFormatId::RawVideo:
	{
		const byte *pCurrent = packet.GetData();
		uint16 nRows = packet.GetSize() / 3 / pStream->codingParameters.video.size.height;
		uint16 rowSize = 3 * pStream->codingParameters.video.size.width;
		for (uint16 y = 0; y < nRows; y++)
		{
			this->outputStream.WriteBytes(pCurrent, rowSize);

			//write padding
			static byte null = 0;
			for (uint8 p = 0; p < rowSize % 4; p++)
				this->outputStream.WriteBytes(&null, 1);

			pCurrent += rowSize;
		}
	}
	break;
	default:
		this->outputStream.WriteBytes(packet.GetData(), packet.GetSize());
		this->imageSize += packet.GetSize();
	}
}
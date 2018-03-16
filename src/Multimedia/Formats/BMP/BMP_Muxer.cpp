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
//Class Header
#include "BMP_Muxer.hpp"
//Local
#include <Std++/Streams/Writers/DataWriter.hpp>
#include "BMP.h"
//Definitions
#define BMP_FILEHEADER_SIZE 14

//Public methods
void BMP_Muxer::Finalize()
{
	uint64 currentOffset = this->refOutput.GetCurrentOffset();

	DataWriter dataWriter(false, this->refOutput);

	//Update file size
	this->refOutput.SetCurrentOffset(this->startOffset + BMP_HEADER_TYPE_SIZE);
	dataWriter.WriteUInt32((uint32)(currentOffset - this->startOffset));

	//Update image size
	this->refOutput.SetCurrentOffset(this->startOffset + BMP_FILEHEADER_SIZE + 20);
	dataWriter.WriteUInt32(this->imageSize);
}

void BMP_Muxer::WriteHeader()
{
	VideoStream *pStream;

	pStream = (VideoStream *)this->GetStream(0);

	this->startOffset = this->refOutput.GetCurrentOffset();

	DataWriter dataWriter(false, this->refOutput);

	//bitmap file header
	this->refOutput.WriteBytes(BMP_HEADER_TYPE, BMP_HEADER_TYPE_SIZE);
	dataWriter.WriteUInt32(0); //file size... updated in finalize
	this->refOutput.WriteUInt16LE(0); //reserved1
	this->refOutput.WriteUInt16LE(0); //reserved2
	dataWriter.WriteUInt32(BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE); //data offset

	//bitmap info header
	dataWriter.WriteUInt32(BMP_INFOHEADER_SIZE);
	dataWriter.WriteUInt32(pStream->width);
	dataWriter.WriteUInt32(-(int32)pStream->height);
	this->refOutput.WriteUInt16LE(1); //number of planes
	this->refOutput.WriteUInt16LE(24); //bits per pixel
	dataWriter.WriteUInt32(0); //compression
	dataWriter.WriteUInt32(0); //image size... updated in finalize
	dataWriter.WriteUInt32(0); //pixels per meter horizontal
	dataWriter.WriteUInt32(0); //pixels per meter vertical
	dataWriter.WriteUInt32(0); //number of color indexes
	dataWriter.WriteUInt32(0); //biClrImportant
}

void BMP_Muxer::WritePacket(const Packet &refPacket)
{
	VideoStream *pStream;

	pStream = (VideoStream *)this->GetStream(0);

	switch(pStream->GetCodec()->GetId())
	{
		case CodecId::BGR24:
		{
			uint16 nRows, rowSize;

			const byte *pCurrent = refPacket.GetData();
			nRows = refPacket.GetSize() / 3 / pStream->height;
			rowSize = 3 * pStream->width;
			for(uint16 y = 0; y < nRows; y++)
			{
				this->refOutput.WriteBytes(pCurrent, rowSize);

				//write padding
				static byte null = 0;
				for(uint8 p = 0; p < rowSize % 4; p++)
					this->refOutput.WriteBytes(&null, 1);

				pCurrent += rowSize;
			}
		}
			break;

		default:
			this->refOutput.WriteBytes(refPacket.GetData(), refPacket.GetSize());
			this->imageSize += refPacket.GetSize();
	}
}
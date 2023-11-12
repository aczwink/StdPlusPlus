/*
 * Copyright (c) 2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/PixelFormat.hpp>
#include "DDS_Muxer.hpp"

//Public methods
void DDS_Muxer::Finalize()
{
	DataWriter dataWriter(false, this->outputStream);

	//TODO: when storing mip maps, these fields need to be changed
	//TODO: mip maps could be realized by using multiple video streams. a bit complicated but anything else would be weird :S
	this->outputStream.SeekTo(this->flagsOffset);
	dataWriter.WriteUInt32(0x1007);

	this->outputStream.SeekTo(this->mipMapCountOffset);
	dataWriter.WriteUInt32(0);

	this->outputStream.SeekTo(this->capsOffset);
	dataWriter.WriteUInt32(0x1000);
}

void DDS_Muxer::WriteHeader()
{
	const auto& vCodec = this->streams[0]->codingParameters.video;

	DataWriter dataWriter(false, this->outputStream);

	dataWriter.WriteUInt32(FOURCC(u8"DDS "));
	dataWriter.WriteUInt32(124); //sizeof(DDS_HEADER)

	this->flagsOffset = this->outputStream.QueryCurrentOffset();
	dataWriter.WriteUInt32(0);

	dataWriter.WriteUInt32(vCodec.size.height);
	dataWriter.WriteUInt32(vCodec.size.width);
	dataWriter.WriteUInt32(this->ComputePitchSize());
	dataWriter.WriteUInt32(0); //depth of volume texture

	this->mipMapCountOffset = this->outputStream.QueryCurrentOffset();
	dataWriter.WriteUInt32(0);

	//reserved data
	for(uint8 i = 0; i < 11; i++)
		dataWriter.WriteUInt32(0);

	this->WritePixelFormat(dataWriter);

	this->capsOffset = this->outputStream.QueryCurrentOffset();
	dataWriter.WriteUInt32(0);

	dataWriter.WriteUInt32(0); //caps2
	dataWriter.WriteUInt32(0); //caps3
	dataWriter.WriteUInt32(0); //caps4
	dataWriter.WriteUInt32(0); //reserved 2
}

void DDS_Muxer::WritePacket(const IPacket &packet)
{
	this->outputStream.WriteBytes(packet.GetData(), packet.GetSize());
}

//Private methods
uint32 DDS_Muxer::ComputePitchSize() const
{
	const auto& codecParameters = this->streams[0]->codingParameters;

	switch(codecParameters.codingFormat->GetId())
	{
		case CodingFormatId::RawVideo:
		{
			ASSERT_EQUALS(1, codecParameters.video.pixelFormat->nPlanes);
			uint8 blockSize = codecParameters.video.pixelFormat->ComputeBlockSize(0);
			return ( codecParameters.video.size.width * (blockSize * 8_u32) + 7_u32 ) / 8_u32;
		}
		case CodingFormatId::S3TC_DXT1:
			return Math::Max( 1_u32, ((codecParameters.video.size.width + 3_u32) / 4_u32) ) * 8_u32;
	}
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

void DDS_Muxer::WritePixelFormat(DataWriter& dataWriter)
{
	const auto& codecParameters = this->streams[0]->codingParameters;
	const auto& vCodec = codecParameters.video;

	uint32 colorSpaceFlags;
	switch(vCodec.pixelFormat->colorSpace)
	{
		case ColorSpace::RGB:
			colorSpaceFlags = 0x40;
			break;
		case ColorSpace::RGBA:
			colorSpaceFlags = 0x41;
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	uint32 fourCC;
	bool isCompressed = true;
	switch(codecParameters.codingFormat->GetId())
	{
		case CodingFormatId::RawVideo:
			isCompressed = false;
			break;
		case CodingFormatId::S3TC_DXT1:
			fourCC = FOURCC(u8"DXT1");
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
	uint32 compressionFlags = isCompressed ? 4 : 0;

	dataWriter.WriteUInt32(32); //sizeof(DDS_PIXELFORMAT)
	dataWriter.WriteUInt32(colorSpaceFlags | compressionFlags);
	dataWriter.WriteUInt32(fourCC);
	ASSERT_EQUALS(1, vCodec.pixelFormat->nPlanes);
	dataWriter.WriteUInt32(vCodec.pixelFormat->ComputeBlockSize(0) * 8_u32);
	dataWriter.WriteUInt32(((1_u32 << vCodec.pixelFormat->colorComponents[0].nBits) - 1_u32) << vCodec.pixelFormat->colorComponents[0].shift);
	dataWriter.WriteUInt32(((1_u32 << vCodec.pixelFormat->colorComponents[1].nBits) - 1_u32) << vCodec.pixelFormat->colorComponents[1].shift);
	dataWriter.WriteUInt32(((1_u32 << vCodec.pixelFormat->colorComponents[2].nBits) - 1_u32) << vCodec.pixelFormat->colorComponents[2].shift);
	if(vCodec.pixelFormat->colorSpace == ColorSpace::RGBA)
		dataWriter.WriteUInt32(((1_u32 << vCodec.pixelFormat->colorComponents[3].nBits) - 1_u32) << vCodec.pixelFormat->colorComponents[3].shift);
	else
		dataWriter.WriteUInt32(0);
}

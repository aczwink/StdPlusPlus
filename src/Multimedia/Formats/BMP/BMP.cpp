/*
 * Copyright (c) 2017-2021 Amir Czwink (amir130@hotmail.de)
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
//Main header
#include "BMP.hpp"
//Local
#include <Std++/Containers/BinaryTreeMap/BinaryTreeMap.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Streams/Writers/DataWriter.hpp>
#include "../../CodingFormatIdMap.hpp"

//Local functions
static _stdxx_::CodingFormatIdMap<uint32> LoadMap()
{
#define ADD_FOURCC_ID(codingFormatId) m.Insert(uint32(codingFormatId), codingFormatId);
	_stdxx_::CodingFormatIdMap<uint32> m;

	ADD_FOURCC_ID(CodingFormatId::MS_MPEG4Part2V2);

	return m;
#undef ADD_FOURCC_ID
}

/*
//Functions
void AddMS_FourCC_VideoCodecs(BinaryTreeSet<CodecId> &refCodecSet)
{
	LoadMap();

	for(const auto &refKV : g_ms_video_fourCC_map)
		refCodecSet.Insert(refKV.value);
}*/

void _stdxx_::ReadBMPHeader(bool &refIsBottomUp, InputStream &inputStream, VideoStream &stream)
{
	DataReader reader(false, inputStream);

	uint32 size = reader.ReadUInt32();
	stream.codingParameters.video.size.width = static_cast<uint16>(reader.ReadUInt32());
	int32 height = reader.ReadInt32();
	inputStream.Skip(2); //planes
	uint16 bitsPerPixel = reader.ReadUInt16();
	uint32 codecTag = reader.ReadUInt32();
	inputStream.Skip(20); //rest of bmp data

	refIsBottomUp = false;

	if(height < 0)
		height = -height;
	else
		refIsBottomUp = true;
	stream.codingParameters.video.size.height = (uint16)height;

	CodingFormatId codingFormatId = CodingFormatId::Unknown;
	switch(codecTag)
	{
		/*
		case 0: //BGR
		{
			switch(bitsPerPixel)
			{
				case 24:
				{
					stream.SetCodingFormat(Codec::GetCodec(CodecId::BGR24));
				}
					break;
				default:
					NOT_IMPLEMENTED_ERROR;
			}
		}
			break;*/
			/*case FOURCC_CINEPAK:
				{
					stream.SetCodec(GetCodec(CODEC_ID_CINEPAK));
				}
				break;
			case FOURCC_H264:
				{
					stream.SetCodec(GetCodec(CODEC_ID_H264));
				}
				break;
			case FOURCC_XVID:
				{
					stream.SetCodec(GetCodec(CODEC_ID_MPEG4PART2));
				}
				break;*/
		//g_ms_video_fourCC_map.Insert(FOURCC("MPG1"), CodecId::MPEG1Video);
	default:
	{
		_stdxx_::CodingFormatIdMap<uint32> codecIdMap = LoadMap();
		codingFormatId = codecIdMap.Get(codecTag);
	}
	}
	stream.SetCodingFormat(codingFormatId);

	ASSERT(size >= 40, "If you see this, report to StdXX");
	if(size > 40)
	{
		inputStream.Skip(size - 40);
	}
}

void _stdxx_::WriteBitmapInfoHeader(VideoStream &stream, OutputStream &outputStream)
{
	DataWriter dataWriter(false, outputStream);

	dataWriter.WriteUInt32(BMP_INFOHEADER_SIZE);
	dataWriter.WriteUInt32(stream.codingParameters.video.size.width);
	dataWriter.WriteUInt32(stream.codingParameters.video.size.height);
	dataWriter.WriteUInt16(1); //number of planes
	dataWriter.WriteUInt16(24); //bits per pixel
	dataWriter.WriteUInt32(static_cast<uint32>(stream.codingParameters.codingFormat->GetId())); //compression
	dataWriter.WriteUInt32(0); //image size...
	dataWriter.WriteUInt32(0); //pixels per meter horizontal
	dataWriter.WriteUInt32(0); //pixels per meter vertical
	dataWriter.WriteUInt32(0); //number of color indexes
	dataWriter.WriteUInt32(0); //biClrImportant
}
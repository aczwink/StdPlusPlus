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
//Main header
#include "BMP.h"
//Local
#include <Std++/Containers/Map/Map.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
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
void AddMS_FourCC_VideoCodecs(FiniteSet<CodecId> &refCodecSet)
{
	LoadMap();

	for(const auto &refKV : g_ms_video_fourCC_map)
		refCodecSet.Insert(refKV.value);
}*/

void ReadBMPHeader(bool &refIsBottomUp, InputStream &inputStream, VideoStream &refStream)
{
	DataReader reader(false, inputStream);

	uint32 size = reader.ReadUInt32();
	refStream.width = reader.ReadUInt32();
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
	refStream.height = (uint16)height;

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
					refStream.SetCodingFormat(Codec::GetCodec(CodecId::BGR24));
				}
					break;
				default:
					NOT_IMPLEMENTED_ERROR;
			}
		}
			break;*/
			/*case FOURCC_CINEPAK:
				{
					refStream.SetCodec(GetCodec(CODEC_ID_CINEPAK));
				}
				break;
			case FOURCC_H264:
				{
					refStream.SetCodec(GetCodec(CODEC_ID_H264));
				}
				break;
			case FOURCC_XVID:
				{
					refStream.SetCodec(GetCodec(CODEC_ID_MPEG4PART2));
				}
				break;*/
		//g_ms_video_fourCC_map.Insert(FOURCC("MPG1"), CodecId::MPEG1Video);
	default:
	{
		_stdxx_::CodingFormatIdMap<uint32> codecIdMap = LoadMap();
		codingFormatId = codecIdMap.GetId(codecTag);
	}
	}
	refStream.SetCodingFormat(codingFormatId);

	ASSERT(size >= 40, "If you see this, report to StdXX");
	if(size > 40)
	{
		inputStream.Skip(size - 40);
	}
}
/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Main header
#include "BMP.h"
//Local
#include <ACStdLib/Containers/Map/Map.hpp>
#include <ACStdLib/Streams/Readers/DataReader.hpp>
#include "../../FourCC.h"

/*
Mircosoft FourCCs:
http://www.faqs.org/rfcs/rfc2361.html
https://msdn.microsoft.com/de-de/library/windows/desktop/bb970509(v=vs.85).aspx
*/

//Global variables
Map<uint32, CodecId> g_ms_video_fourCC_map;

//Local functions
static void LoadMap()
{
	static bool loaded = false;

	if(!loaded)
	{
		loaded = true;

		g_ms_video_fourCC_map.Insert(FOURCC("MP42"), CodecId::MS_MPEG4Part2); // Microsoft MPEG-4 version 2
		g_ms_video_fourCC_map.Insert(FOURCC("MPG1"), CodecId::MPEG1Video);
	}
}

//Functions
void AddMS_FourCC_VideoCodecs(FiniteSet<CodecId> &refCodecSet)
{
	LoadMap();

	for(const auto &refKV : g_ms_video_fourCC_map)
		refCodecSet.Insert(refKV.value);
}

void ReadBMPHeader(bool &refIsBottomUp, InputStream &inputStream, VideoStream &refStream)
{
	uint16 bitsPerPixel;
	int32 height;
	uint32 size, codecTag;

	DataReader reader(false, inputStream);

	LoadMap();

	size = reader.ReadUInt32();
	refStream.width = reader.ReadUInt32();
	height = reader.ReadInt32();
	inputStream.Skip(2); //planes
	bitsPerPixel = reader.ReadUInt16();
	codecTag = reader.ReadUInt32();
	inputStream.Skip(20); //rest of bmp data

	refIsBottomUp = false;

	if(height < 0)
		height = -height;
	else
		refIsBottomUp = true;
	refStream.height = (uint16)height;

	switch(codecTag)
	{
		case 0: //BGR
		{
			switch(bitsPerPixel)
			{
				case 24:
				{
					refStream.SetCodec(Codec::GetCodec(CodecId::BGR24));
				}
					break;
				default:
					ASSERT(0);
			}
		}
			break;
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
	}

	if(refStream.GetCodec() == nullptr && g_ms_video_fourCC_map.Contains(codecTag))
		refStream.SetCodec(g_ms_video_fourCC_map[codecTag]);

	ASSERT(size >= 40);
	if(size > 40)
	{
		inputStream.Skip(size - 40);
	}
}
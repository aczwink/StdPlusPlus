//Main header
#include "BMP.h"
//Local
#include "../../FourCC.h"

/*
Mircosoft FourCCs:
http://www.faqs.org/rfcs/rfc2361.html
https://msdn.microsoft.com/de-de/library/windows/desktop/bb970509(v=vs.85).aspx
*/

//Global variables
static CMap<uint32, ECodecId> g_ms_video_fourCC_map;

//Local functions
static void LoadMap()
{
	static bool loaded = false;

	if(!loaded)
	{
		loaded = true;
		
		g_ms_video_fourCC_map.Insert(FOURCC("MP42"), ECodecId::MS_MPEG4Part2); // Microsoft MPEG-4 version 2
		g_ms_video_fourCC_map.Insert(FOURCC("MPG1"), ECodecId::MPEG1Video);
	}
}

//Functions
void AddMS_FourCC_VideoCodecs(CFiniteSet<ECodecId> &refCodecSet)
{
	LoadMap();

	for(const auto &refKV : g_ms_video_fourCC_map)
		refCodecSet.Insert(refKV.value);
}

void ReadBMPHeader(bool &refIsBottomUp, AInputStream &refInput, CVideoStream &refStream)
{
	uint16 bitsPerPixel;
	int32 height;
	uint32 size, codecTag;

	LoadMap();
	
	size = refInput.ReadUInt32LE();
	refStream.width = refInput.ReadUInt32LE();
	height = refInput.ReadInt32LE();
	refInput.Skip(2); //planes
	bitsPerPixel = refInput.ReadUInt16LE();
	codecTag = refInput.ReadUInt32LE();
	refInput.Skip(20); //rest of bmp data

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
					refStream.SetCodec(ICodec::GetCodec(ECodecId::BGR24));
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
		refInput.Skip(size - 40);
	}
}
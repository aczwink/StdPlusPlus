//Main header
#include "Matroska.h"

//Global variables
static CMap<C8BitString, ECodecId> g_matroskaCodecStringMap;

//Local functions
static void LoadMap()
{
	static bool loaded = false;

	if(!loaded)
	{
		loaded = true;

		//Audio codecs
		g_matroskaCodecStringMap.Insert("A_AAC", ECodecId::AAC);
		g_matroskaCodecStringMap.Insert("A_MPEG/L2", ECodecId::MP2);
		g_matroskaCodecStringMap.Insert("A_MPEG/L3", ECodecId::MP3);

		//Video codecs
		g_matroskaCodecStringMap.Insert("V_MPEG1", ECodecId::MPEG1Video);
		g_matroskaCodecStringMap.Insert("V_MPEG4/ISO/AVC", ECodecId::H264);
	}
}

//Namespace functions
ECodecId Matroska::MapCodecString(const C8BitString &refCodecString)
{
	LoadMap();

	if(g_matroskaCodecStringMap.Contains(refCodecString))
		return g_matroskaCodecStringMap[refCodecString];

	return ECodecId::Unknown;
}

C8BitString Matroska::MapToCodecString(ECodecId codecId)
{
	LoadMap();

	for(const auto &refKV : g_matroskaCodecStringMap)
		if(refKV.value == codecId)
			return refKV.key;
	
	return C8BitString();
}
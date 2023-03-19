//Main header
#include "WAVE.h"
//Namespaces

//Global variables
static CMap<uint16, ECodecId> g_ms_audio_twoCC_map;

//Local functions
static void LoadMap()
{
	static bool loaded = false;

	if(!loaded)
	{
		loaded = true;

		g_ms_audio_twoCC_map.Insert(1, ECodecId::PCM_S16LE); //WAVE_FORMAT_PCM
		g_ms_audio_twoCC_map.Insert(3, ECodecId::PCM_Float32LE); //WAVE_FORMAT_IEEE_FLOAT
		g_ms_audio_twoCC_map.Insert(0x50, ECodecId::MP2); //TODO: could also be layer 1
	}
}

//Functions
void AddMS_TwoCC_AudioCodecs(CFiniteSet<ECodecId> &refCodecSet)
{
	LoadMap();

	for(const auto &refKV : g_ms_audio_twoCC_map)
		refCodecSet.Insert(refKV.value);
}

uint16 MapToTwoCC(ECodecId codecId)
{
	for(const auto &refKV : g_ms_audio_twoCC_map)
		if(refKV.value == codecId)
			return refKV.key;

	NOT_IMPLEMENTED_ERROR;
	return -1;
}
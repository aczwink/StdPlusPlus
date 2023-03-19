#pragma once
#include <ACStdLib.h>
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;
//Local
#include "../../FourCC.h"

//Definitions
#define WAVE_RIFFCHUNK_CHUNKID FOURCC("RIFF")
#define WAVE_RIFFCHUNK_RIFFTYPE FOURCC("WAVE")
#define WAVE_FORMATCHUNK_CHUNKID FOURCC("fmt ")
#define WAVE_DATACHUNK_CHUNKID FOURCC("data")

//Functions
void AddMS_TwoCC_AudioCodecs(CFiniteSet<ECodecId> &refCodecSet);
uint16 MapToTwoCC(ECodecId codecId);
#pragma once
#include <ACStdLib.h>
using namespace ACStdLib;
//Definitions
#define MPEG1_AUDIO_SUBBANDLIMIT 32

/*
Sources for Implementation:
ISO IEC 11172-3 (MPEG1)
ISO IEC 13818-3 (MPEG2)
*/

//Structs
struct SMPEGAudioFrameHeader
{
	bool isMpeg1;
	uint8 layer;
	bool crcFollows;
	uint32 bitRate;
	uint16 sampleRate;
	uint8 nChannels;
	bool isMS_Stereo;
	bool isIntensityStereo;
	uint16 frameSize;
	uint16 mainDataSize;
	uint8 sideInfoSize;
};

//Functions
void ApplySynthesisSubbandFilter(const float64(&refBand)[MPEG1_AUDIO_SUBBANDLIMIT], float64(&refV)[1024], float32 *pOutData);
bool FindNextFrameHeader(uint32 &refState, ASeekableInputStream &refInput);
void ParseFrameHeader(uint32 frameHeader, SMPEGAudioFrameHeader &refFrameInfo);
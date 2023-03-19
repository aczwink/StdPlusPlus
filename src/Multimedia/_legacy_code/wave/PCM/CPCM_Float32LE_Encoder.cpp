/*//Class Header
#include "__CPCM_Float32LE_Encoder.h"
//Global
#include <iostream>
//Local
#include "CAudioFrame.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

//Constructor
CPCM_Float32LE_Encoder::CPCM_Float32LE_Encoder()
{
	this->nChannels = 0;
	this->sampleRate = 0;
}

//Public Functions
void CPCM_Float32LE_Encoder::Encode(SPacket *pPacket, const IFrame *pFrame) const
{
	float *pOutput;
	CAudioFrame *pAFrame;
	CAudioFrame convertedFrame;
	
	pAFrame = (CAudioFrame *)pFrame;
	
	ASSERT(pFrame->GetType() == DATATYPE_AUDIO);
	
	if(pAFrame->GetSampleFormat() != SAMPLEFORMAT_FLOAT)
	{
		pAFrame->Convert(convertedFrame, SAMPLEFORMAT_FLOAT);
		pAFrame = &convertedFrame;
	}
	
	pPacket->size = pAFrame->GetSize();
	pPacket->pData = (byte *)malloc(pPacket->size);
	pOutput = (float *)pPacket->pData;
	
	switch(pAFrame->GetChannelLayout())
	{
	case CHANNELLAYOUT_MONO:
		{
			const float *pInput;

			pInput = (const float *)pAFrame->GetData(AUDIOFRAME_MONO);
			repeat(pAFrame->GetNumberOfSamplesPerChannel(), i)
			{
				*pOutput++ = *pInput++;
			}
		}
		break;
	case CHANNELLAYOUT_STEREO:
		{
			const float *pLeft, *pRight;

			pLeft = (const float *)pAFrame->GetData(AUDIOFRAME_LEFT);
			pRight = (const float *)pAFrame->GetData(AUDIOFRAME_RIGHT);
			repeat(pAFrame->GetNumberOfSamplesPerChannel(), i)
			{
				*pOutput++ = *pLeft++;
				*pOutput++ = *pRight++;
			}
			break;
		}
	}
}

uint16 CPCM_Float32LE_Encoder::GetBitsPerSample() const
{
	return 32;
}

ECodecId CPCM_Float32LE_Encoder::GetId() const
{
	return CODEC_ID_PCM_FLOAT32LE;
}

CString CPCM_Float32LE_Encoder::GetName() const
{
	return "PCM 32-bit IEEE Floating-point little-endian (in range [-1, 1])";
}

void CPCM_Float32LE_Encoder::Release()
{
}*/
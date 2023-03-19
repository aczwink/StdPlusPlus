//Class Header
#include "CAudioFrame.h"
//Global
#include <iostream>
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

//Constructor
CAudioFrame::CAudioFrame(EChannelLayout channelLayout, ESampleFormat sampleFormat, uint32 nSamplesPerChannel)
{	
	MemZero(this->pData, sizeof(this->pData));
	
	this->sampleFormat = sampleFormat;
}

//Public methods
CAudioFrame *CAudioFrame::Convert(ESampleFormat outputSampleFormat) const
{
	CAudioFrame *pFrame;
	
	pFrame = new CAudioFrame(this->channelLayout, outputSampleFormat, this->nSamplesPerChannel);
	
	switch(this->sampleFormat)
	{
	case SAMPLEFORMAT_FLOAT_32BIT:
		{
			switch(outputSampleFormat)
			{
			case SAMPLEFORMAT_FLOAT_32BIT:
				{
					delete pFrame;
					return this->Copy();
				}
				break;
			}
		}
		break;
	case SAMPLEFORMAT_SIGNED_16BIT:
		{
			switch(outputSampleFormat)
			{
			case SAMPLEFORMAT_FLOAT_32BIT:
				{
					switch(this->channelLayout)
					{
					case CHANNELLAYOUT_MONO:
						{
							const int16 *pInput;
							float *pOutput;

							pInput = (int16 *)this->pData[AUDIOFRAME_CHANNEL_MONO];
							pOutput = (float *)pFrame->pData[AUDIOFRAME_CHANNEL_MONO];
							repeat(this->nSamplesPerChannel, i)
							{
								*pOutput++ = (float)(*pInput++) / 32767;
							}
						}
						break;
					case CHANNELLAYOUT_STEREO:
						{
							ASSERT(0);
						}
						break;
					}
				}
				break;
			case SAMPLEFORMAT_SIGNED_16BIT:
				{
					delete pFrame;
					return this->Copy();
				}
				break;
			}
		}
		break;
	}

	return pFrame;
}

CAudioFrame *CAudioFrame::Copy() const
{
	CAudioFrame *pFrame;
	
	pFrame = new CAudioFrame(this->channelLayout, this->sampleFormat, this->nSamplesPerChannel);
	
	switch(this->sampleFormat)
	{
	case SAMPLEFORMAT_FLOAT_32BIT:
		{
			ASSERT(0);
		}
		break;
	case SAMPLEFORMAT_SIGNED_16BIT:
		{
			switch(this->channelLayout)
			{
			case CHANNELLAYOUT_MONO:
				{
					int16 *pInput, *pOutput;

					pInput = (int16 *)this->pData[AUDIOFRAME_CHANNEL_MONO];
					pOutput = (int16 *)pFrame->pData[AUDIOFRAME_CHANNEL_MONO];

					repeat(this->nSamplesPerChannel, i)
					{
						*pOutput++ = *pInput++;
					}
				}
				break;
			case CHANNELLAYOUT_STEREO:
				{
					int16 *pInLeft, *pInRight, *pOutLeft, *pOutRight;

					pInLeft = (int16 *)this->pData[AUDIOFRAME_CHANNEL_LEFT];
					pInRight = (int16 *)this->pData[AUDIOFRAME_CHANNEL_RIGHT];
					pOutLeft = (int16 *)pFrame->pData[AUDIOFRAME_CHANNEL_LEFT];
					pOutRight = (int16 *)pFrame->pData[AUDIOFRAME_CHANNEL_RIGHT];

					repeat(this->nSamplesPerChannel, i)
					{
						*pOutLeft++ = *pInLeft++;
						*pOutRight++ = *pInRight++;
					}
				}
				break;
			default:
				ASSERT(0);
			}
		}
		break;
	default:
		ASSERT(0);
	}

	return pFrame;
}
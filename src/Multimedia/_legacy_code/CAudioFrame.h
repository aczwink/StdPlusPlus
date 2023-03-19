#pragma once
//Local
#include "Definitions.h"
#include "IFrame.h"
//Definitions
#define AUDIOFRAME_MAXCHANNELS 2
#define AUDIOFRAME_CHANNEL_MONO 0
#define AUDIOFRAME_CHANNEL_LEFT 0
#define AUDIOFRAME_CHANNEL_RIGHT 1

namespace SJCMMLib
{
	class SJCMMLIB_API CAudioFrame : public IFrame
	{
	private:
		//Variables
		ESampleFormat sampleFormat;
	public:
		//Constructor
		CAudioFrame(EChannelLayout channelLayout, ESampleFormat sampleFormat);
		//Functions
		CAudioFrame *Convert(ESampleFormat outputSampleFormat) const;
		CAudioFrame *Copy() const;
		//Inline
		inline ESampleFormat GetSampleFormat() const
		{
			return this->sampleFormat;
		}
	};
}
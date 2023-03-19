#pragma once
//Local
#include "../Definitions.h"
#include "Definitions.h"

namespace ACStdLib
{
	namespace Multimedia
	{
		enum class EChannel
		{
			Left = 0,
			Right = 1
		};

		enum class EAudioSampleType
		{
			PCM_Float32LE,
			PCM_S16LE,
		};
		
		class ACSTDLIB_API AAudioBuffer
		{
		private:
			//Members
			EChannelLayout channelLayout;
			uint32 nSamplesPerChannel;
		
		public:
			//Constructor
			inline AAudioBuffer(EChannelLayout channelLayout, uint32 nSamplesPerChannel)
			{
				this->channelLayout = channelLayout;
				this->nSamplesPerChannel = nSamplesPerChannel;
			}
			
			//Destructor
			virtual ~AAudioBuffer() {}
			
			//Abstract
			virtual uint16 GetSampleSize() const = NULL;
			virtual EAudioSampleType GetSampleType() const = NULL;

			//Methods
			uint8 GetNumberOfChannels() const;

			//Inline
			inline EChannelLayout GetChannelLayout() const
			{
				return this->channelLayout;
			}

			inline uint32 GetChannelSize() const
			{
				return this->nSamplesPerChannel * this->GetSampleSize();
			}

			inline uint32 GetNumberOfSamplesPerChannel() const
			{
				return this->nSamplesPerChannel;
			}

			inline uint32 GetSize() const
			{
				return this->GetNumberOfChannels() * this->GetChannelSize();
			}
		};
	}
}
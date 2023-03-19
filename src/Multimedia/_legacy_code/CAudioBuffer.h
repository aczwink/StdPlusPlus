#pragma once
//Local
#include "AAudioBuffer.h"

namespace ACStdLib
{
	namespace Multimedia
	{
		template<typename SampleType>
		class ACSTDLIB_API CAudioBuffer : public AAudioBuffer
		{
		private:
			//Members
			SampleType *pChannels[2];

			//Methods
			void AllocateMemory();
			template<typename InputSampleType>
			void Resample(const CAudioBuffer<InputSampleType> &refBuffer);
		
		public:
			//Constructors
			inline CAudioBuffer(EChannelLayout channelLayout, uint32 nSamplesPerChannel) : AAudioBuffer(channelLayout, nSamplesPerChannel)
			{
				this->AllocateMemory();
			}
			CAudioBuffer(const AAudioBuffer &refBuffer);
			
			//Destructor
			~CAudioBuffer();

			//Methods
			EAudioSampleType GetSampleType() const;
			
			//Inline
			inline SampleType *GetChannel(EChannel channel)
			{
				return this->pChannels[(uint32)channel];
			}

			inline const SampleType *GetChannel(EChannel channel) const
			{
				return this->pChannels[(uint32)channel];
			}

			inline uint16 GetSampleSize() const
			{
				return sizeof(SampleType);
			}
		};
	}
}
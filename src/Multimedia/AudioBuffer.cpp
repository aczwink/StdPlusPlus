/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of Std++.
 *
 * Std++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Std++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Std++.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include <Std++/Multimedia/AudioBuffer.hpp>
//Local
#include <Std++/Debug.hpp>
#include <Std++/Integer.hpp>
#include <Std++/Memory.h>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::Multimedia;

//Constructors
template<typename SampleType>
AudioBuffer<SampleType>::AudioBuffer(const AbstractAudioBuffer &refBuffer) : AbstractAudioBuffer(refBuffer.GetChannelLayout(), refBuffer.GetNumberOfSamplesPerChannel())
{
	switch(refBuffer.GetSampleType())
	{
		case EAudioSampleType::PCM_Float32LE:
			this->Resample((const AudioBuffer<float32> &)refBuffer);
			break;
		default:
			NOT_IMPLEMENTED_ERROR;
	}
}

//Destructor
template<typename SampleType>
AudioBuffer<SampleType>::~AudioBuffer()
{
	if(this->pChannels[0])
		MemFree(this->pChannels[0]);
	if(this->pChannels[1])
		MemFree(this->pChannels[1]);
}

//Private methods
template<typename SampleType>
void AudioBuffer<SampleType>::AllocateMemory()
{
	switch(this->GetChannelLayout())
	{
		case ChannelLayout::Mono:
			this->pChannels[(uint32)Channel::Left] = (SampleType *)MemAlloc(this->GetChannelSize());
			this->pChannels[(uint32)Channel::Right] = nullptr;
			break;
		case ChannelLayout::Stereo:
			this->pChannels[(uint32)Channel::Left] = (SampleType *)MemAlloc(this->GetChannelSize());
			this->pChannels[(uint32)Channel::Right] = (SampleType *)MemAlloc(this->GetChannelSize());
			break;
		default:
			NOT_IMPLEMENTED_ERROR;
	}
}

//Resampling
template<>
template<>
void AudioBuffer<float32>::Resample(const AudioBuffer<float32>& refBuffer)
{
	const float32 *pInput;
	float32 *pOutput;

	this->AllocateMemory();

	for(uint8 ch = 0; ch < this->GetNumberOfChannels(); ch++)
	{
		pInput = refBuffer.GetChannel((Channel)ch);
		pOutput = this->GetChannel((Channel)ch);

		MemCopy(pOutput, pInput, this->GetChannelSize());
	}
}

template<>
template<>
void AudioBuffer<int16>::Resample(const AudioBuffer<float32>& refBuffer)
{
	const float32 *pInput;
	int16 *pOutput;

	this->AllocateMemory();

	for(uint8 ch = 0; ch < this->GetNumberOfChannels(); ch++)
	{
		pInput = refBuffer.GetChannel((Channel)ch);
		pOutput = this->GetChannel((Channel)ch);
		for(uint32 i = 0; i < this->GetNumberOfSamplesPerChannel(); i++)
		{
			pOutput[i] = (int16)(pInput[i] * Integer<int16>::Max());
		}
	}
}

//Public methods
template<>
EAudioSampleType AudioBuffer<float32>::GetSampleType() const
{
	return EAudioSampleType::PCM_Float32LE;
}
template<>
EAudioSampleType AudioBuffer<int16>::GetSampleType() const
{
	return EAudioSampleType::PCM_S16LE;
}

//Explicit instantiation
template STDPLUSPLUS_API class AudioBuffer<float32>;
template STDPLUSPLUS_API class AudioBuffer<int16>;
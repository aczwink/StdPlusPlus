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
#include <Std++/Memory.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

//Destructor
AudioBuffer::~AudioBuffer()
{
	for (uint8 i = 0; i < AudioSampleFormat::MAX_PLANES; i++)
	{
		MemFree(this->planes[i]);
	}
}

//Public methods
AudioBuffer *AudioBuffer::Resample(const AudioSampleFormat &fromFormat, const AudioSampleFormat &toFormat) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

//Private methods
void AudioBuffer::AllocateMemory(const AudioSampleFormat &sampleFormat)
{
	uint8 i;
	for (i = 0; i < sampleFormat.nPlanes; i++)
	{
		this->planeSizes[i] = sampleFormat.ComputePlaneSize(i, this->nSamplesPerChannel);
		this->planeBlockSizes[i] = sampleFormat.ComputeBlockSize(i);
		this->planes[i] = MemAlloc(this->planeSizes[i]);
	}
	for (; i < AudioSampleFormat::MAX_PLANES; i++)
	{
		this->planeSizes[i] = 0;
		this->planeBlockSizes[i] = 0;
		this->planes[i] = nullptr;
	}
}

/*
//Constructors
template<typename SampleType>
AudioBuffer<SampleType>::AudioBuffer(const AbstractAudioBuffer &refBuffer) : AbstractAudioBuffer(refBuffer.GetChannelLayout(), refBuffer.GetNumberOfSamplesPerChannel())
{
	switch(refBuffer.GetSampleType())
	{
		case AudioSampleFormat::Float32:
			this->Resample((const AudioBuffer<float32> &)refBuffer);
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
AudioSampleFormat AudioBuffer<float32>::GetSampleType() const
{
	return AudioSampleFormat::Float32;
}
template<>
AudioSampleFormat AudioBuffer<int16>::GetSampleType() const
{
	return AudioSampleFormat::S16;
}

//Explicit instantiation
template STDPLUSPLUS_API class StdXX::Multimedia::AudioBuffer<float32>;
template STDPLUSPLUS_API class StdXX::Multimedia::AudioBuffer<int16>;
*/
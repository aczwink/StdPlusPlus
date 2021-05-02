/*
 * Copyright (c) 2017-2018,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Signed.hpp>
#include <Std++/Mathematics.hpp>
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
	AudioBuffer* result = new AudioBuffer(this->GetNumberOfSamplesPerChannel(), toFormat);

	ASSERT(fromFormat.nChannels == toFormat.nChannels, u8"TODO: implement channel mixing");

	for (uint8 ch = 0; ch < fromFormat.nChannels; ch++)
	{
		const auto &srcChannel = fromFormat.channels[ch];
		const auto &destChannel = toFormat.channels[ch];
		for (uint32 i = 0; i < this->GetNumberOfSamplesPerChannel(); i++)
		{
			const byte* src = (const byte*)this->GetPlane(srcChannel.planeIndex);
			src += i * this->GetPlaneBlockSize(srcChannel.planeIndex) + srcChannel.offset;

			byte* dst = (byte*)result->GetPlane(destChannel.planeIndex);
			dst += i * result->GetPlaneBlockSize(destChannel.planeIndex) + destChannel.offset;

			this->ResampleSingleSample(src, dst, fromFormat.sampleType, toFormat.sampleType);
		}
	}

	return result;
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

void AudioBuffer::ResampleSingleSample(const void *source, void *dest, AudioSampleType sourceSampleType, AudioSampleType targetSampleType) const
{
	switch(sourceSampleType)
	{
		case AudioSampleType::Float:
		{
			switch(targetSampleType)
			{
				case AudioSampleType::S16:
				{
					float32 src_sample = *(const float32*)source;
					int16* dst_typed = (int16*)dest;
					*dst_typed = Math::Clamp(int16(src_sample * Signed<int16>::Max()), Signed<int16>::Min(), Signed<int16>::Max());
				}
				break;
				default:
					NOT_IMPLEMENTED_ERROR; //TODO: implement me
			}
		}
		break;
		case AudioSampleType::S16:
		{
			switch(targetSampleType)
			{
				case AudioSampleType::S16:
					*(int16*)dest = *(const int16*)source;
					break;
				default:
					NOT_IMPLEMENTED_ERROR; //TODO: implement me
			}
		}
		break;
		case AudioSampleType::U8:
		{
			switch(targetSampleType)
			{
				case AudioSampleType::U8:
					*(uint8*)dest = *(const uint8*)source;
				break;
				default:
					NOT_IMPLEMENTED_ERROR; //TODO: implement me
			}
		}
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
}
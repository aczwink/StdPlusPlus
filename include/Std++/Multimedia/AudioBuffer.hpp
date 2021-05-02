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
#pragma once
//Local
#include "AudioSampleFormat.hpp"

namespace StdXX
{
    namespace Multimedia
    {
		class STDPLUSPLUS_API AudioBuffer
		{
		public:
			//Constructors
            inline AudioBuffer(uint32 nSamplesPerChannel, const AudioSampleFormat &sampleFormat) : nSamplesPerChannel(nSamplesPerChannel)
            {
                this->AllocateMemory(sampleFormat);
            }

            //Destructor
            ~AudioBuffer();

			//Methods
			AudioBuffer *Resample(const AudioSampleFormat &fromFormat, const AudioSampleFormat &toFormat) const;

			//Inline
			inline uint32 GetNumberOfSamplesPerChannel() const
			{
				return this->nSamplesPerChannel;
			}

			inline void *GetPlane(uint8 i)
			{
				return this->planes[i];
			}

			inline const void *GetPlane(uint8 planeIndex) const
			{
				return this->planes[planeIndex];
			}

			inline uint8 GetPlaneBlockSize(uint8 planeIndex) const
			{
				return this->planeBlockSizes[planeIndex];
			}

			inline uint32 GetPlaneSize(uint8 planeIndex) const
			{
				return this->planeSizes[planeIndex];
			}

		private:
			//Members
			uint32 nSamplesPerChannel;
			void *planes[AudioSampleFormat::MAX_PLANES];
			uint32 planeSizes[AudioSampleFormat::MAX_PLANES];
			uint8 planeBlockSizes[AudioSampleFormat::MAX_PLANES];

			//Methods
			void AllocateMemory(const AudioSampleFormat &sampleFormat);
			void ResampleSingleSample(const void* source, void* dest, AudioSampleType sourceSampleType, AudioSampleType targetSampleType) const;
        };
    }
}
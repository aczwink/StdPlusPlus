/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Definitions.h>

namespace StdXX
{
	namespace Multimedia
	{
		enum class SpeakerPosition
		{
			Unknown,
			Front_Center,
			Front_Left,
			Front_Right,
		};

		enum class AudioSampleType
		{
			Float,
			S16,
		};

		class STDPLUSPLUS_API AudioSampleFormat
		{
		public:
			//Constants
			static const uint8 MAX_CHANNELS = 2;
			static const uint8 MAX_PLANES = MAX_CHANNELS;

			//Members
			uint8 nPlanes;
			uint8 nChannels;
			AudioSampleType sampleType;
			struct
			{
				/**
				 * In which plane is the sample found.
				 */
				uint8 planeIndex;
				/**
				 * Specifies the number of bytes that are discarded, before the sample.
				 */
				uint8 offset;
				/**
				 * Speaker position that the channel should targeted to.
				 */
				SpeakerPosition speaker;
			} channels[MAX_CHANNELS];

			//Constructors
			inline AudioSampleFormat(AudioSampleType sampleType) : nPlanes(0), nChannels(0), sampleType(sampleType)
			{
			}
			
			AudioSampleFormat(uint8 nChannels, AudioSampleType sampleType, bool planar); //Create standard layout

			//Operators
			inline bool operator==(const AudioSampleFormat &other) const
			{
				return (this->nPlanes == other.nPlanes) && (this->nChannels == other.nChannels) && (this->channels == other.channels);
			}

			inline bool operator!=(const AudioSampleFormat& other) const
			{
				return (*this == other);
			}

			//Methods
			uint8 ComputeBlockSize(uint8 planeIndex) const;
			uint8 GetSampleSize() const;

			//Inline
			inline uint32 ComputePlaneSize(uint8 planeIndex, uint32 nSamplesPerChannel) const
			{
				return this->ComputeBlockSize(planeIndex) * nSamplesPerChannel;
			}

			inline bool IsPlanar() const
			{
				return this->nPlanes > 1;
			}
		};
	}
}
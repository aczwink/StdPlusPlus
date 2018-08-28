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
#include "AbstractAudioBuffer.hpp"

namespace StdXX
{
    namespace Multimedia
    {
		/**
		 * A planar audio buffer.
		 */
        template<typename SampleType>
        class STDPLUSPLUS_API AudioBuffer : public AbstractAudioBuffer
        {
        private:
            //Members
            SampleType *pChannels[2];

            //Methods
            void AllocateMemory();
            template<typename InputSampleType>
            void Resample(const AudioBuffer<InputSampleType> &refBuffer);

        public:
            //Constructors
            inline AudioBuffer(ChannelLayout channelLayout, uint32 nSamplesPerChannel) : AbstractAudioBuffer(channelLayout, nSamplesPerChannel)
            {
                this->AllocateMemory();
            }
            AudioBuffer(const AbstractAudioBuffer &refBuffer);

            //Destructor
            ~AudioBuffer();

            //Methods
			AudioSampleFormat GetSampleType() const;

            //Inline
            inline SampleType *GetChannel(Channel channel)
            {
                return this->pChannels[(uint32)channel];
            }

            inline const SampleType *GetChannel(Channel channel) const
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
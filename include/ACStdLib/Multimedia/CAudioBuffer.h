/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
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
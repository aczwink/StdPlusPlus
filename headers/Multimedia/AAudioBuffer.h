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
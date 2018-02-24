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
#include "../Definitions.h"
#include "EnumTypes.hpp"

namespace StdPlusPlus
{
    namespace Multimedia
    {
        enum class Channel
        {
            Left = 0,
            Right = 1
        };

        enum class EAudioSampleType
        {
            PCM_Float32LE,
            PCM_S16LE,
        };

        class STDPLUSPLUS_API AbstractAudioBuffer
        {
        private:
            //Members
            ChannelLayout channelLayout;
            uint32 nSamplesPerChannel;

        public:
            //Constructor
            inline AbstractAudioBuffer(ChannelLayout channelLayout, uint32 nSamplesPerChannel)
            {
                this->channelLayout = channelLayout;
                this->nSamplesPerChannel = nSamplesPerChannel;
            }

            //Destructor
            virtual ~AbstractAudioBuffer() {}

            //Abstract
            virtual uint16 GetSampleSize() const = 0;
            virtual EAudioSampleType GetSampleType() const = 0;

            //Methods
            uint8 GetNumberOfChannels() const;

            //Inline
            inline ChannelLayout GetChannelLayout() const
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
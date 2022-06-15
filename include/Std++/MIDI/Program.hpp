/*
 * Copyright (c) 2022 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Array/DynamicArray.hpp>
#include <Std++/Streams/InputStream.hpp>
#include <Std++/Buffers/DynamicByteBuffer.hpp>
#include "Event.hpp"

namespace StdXX::MIDI
{
    class Program
    {
    public:
        //Constructor
        inline Program(uint16 timeDivision) : timeDivision(timeDivision)
        {
        }

        //Properties
        inline const DynamicArray<MetaEvent>& MetaTrack() const
        {
            return this->metaEvents;
        }

        inline uint32 NumberOfChannelTracks() const
        {
            return this->channelTracks.GetNumberOfElements();
        }

        inline uint16 TimeDivision() const
        {
            return this->timeDivision;
        }

        //Inline
        inline void AddChannelMessage(ChannelMessageType type, uint16 trackNumber, uint64 t, uint8 value1)
        {
            this->AddChannelMessage(type, trackNumber, t, value1, 0);
        }

        inline void AddChannelMessage(ChannelMessageType type, uint16 trackNumber, uint64 t, uint8 value1, uint8 value2)
        {
            if(this->channelTracks.GetNumberOfElements() <= trackNumber)
                this->channelTracks.Resize(trackNumber+1);
            this->channelTracks[trackNumber].Push({type, t, value1, value2});
        }

        inline void AddMetaEvent(MetaEventType type, uint64 t, const DynamicByteBuffer& data)
        {
            this->metaEvents.Push({type, t, data});
        }

        inline const DynamicArray<ChannelMessage>& GetChannelTrack(uint32 channelNumber) const
        {
            return this->channelTracks[channelNumber];
        }

        //Functions
        static Program Load(InputStream& inputStream);

    private:
        //Variables
        uint16 timeDivision;
        DynamicArray<DynamicArray<ChannelMessage>> channelTracks;
        DynamicArray<MetaEvent> metaEvents;
    };
}
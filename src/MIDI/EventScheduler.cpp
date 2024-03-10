/*
 * Copyright (c) 2022-2024 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/MIDI/EventScheduler.hpp>
//Local
#include <Std++/Containers/Array/FixedArray.hpp>
#include <Std++/Time/TimeMisc.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::MIDI;

enum class TrackType
{
    ChannelTrack,
    MetaTrack,
};

struct NextEventResult
{
    TrackType trackType;
    uint32 channelTrackNumber;
    uint32 trackIndex;
    uint64 deltaTime;
};

struct ScheduleState
{
    uint32 metaTrackIdx = 0;
    FixedArray<uint32> channelIndices;
    uint64 t = 0;

    //Constructor
    inline ScheduleState(uint32 nChannelTracks) : channelIndices(nChannelTracks)
    {
        for(uint32& channelIndex : this->channelIndices)
            channelIndex = 0;
    }

    //Methods
    bool FindNextEventTrack(const Program& program, NextEventResult& result)
    {
        int64 smallestT = Signed<int64>::Min();

        const auto& metaTrack = program.MetaTrack();
        if(this->metaTrackIdx < metaTrack.GetNumberOfElements())
        {
            smallestT = metaTrack[this->metaTrackIdx].timeStamp;
            result.trackType = TrackType::MetaTrack;
            result.trackIndex = this->metaTrackIdx;
        }

        for(uint32 channelNumber = 0; channelNumber < this->channelIndices.GetNumberOfElements(); channelNumber++)
        {
            uint32 channelIndex = this->channelIndices[channelNumber];
            const auto& channelTrack = program.GetChannelTrack(channelNumber);
            if(channelIndex < channelTrack.GetNumberOfElements())
            {
                if(channelTrack[channelIndex].timeStamp < smallestT)
                {
                    smallestT = channelTrack[channelIndex].timeStamp;
                    result.trackType = TrackType::ChannelTrack;
                    result.channelTrackNumber = channelNumber;
                    result.trackIndex = channelIndex;
                }
            }
        }

        result.deltaTime = smallestT - this->t;
        return smallestT >= 0;
    }

    void Update(const NextEventResult& nextEventResult)
    {
        this->t += nextEventResult.deltaTime;

        switch(nextEventResult.trackType)
        {
            case TrackType::ChannelTrack:
                this->channelIndices[nextEventResult.channelTrackNumber]++;
                break;
            case TrackType::MetaTrack:
                this->metaTrackIdx++;
                break;
        }
    }
};

//Public methods
void EventScheduler::Schedule(const bool& continuePlaying)
{
    ScheduleState state(this->program.NumberOfChannelTracks());

    NextEventResult nextEvent;
    while(continuePlaying and state.FindNextEventTrack(this->program, nextEvent))
    {
        uint64 delta_us = (nextEvent.deltaTime / this->ticksPerMicrosecond).DivideAndRoundDown();
        Sleep(delta_us * 1000);

        switch(nextEvent.trackType)
        {
            case TrackType::ChannelTrack:
            {
                const auto& nextChannelEvent = this->program.GetChannelTrack(nextEvent.channelTrackNumber)[nextEvent.trackIndex];

                switch(nextChannelEvent.type)
                {
                    case ChannelMessageType::ControlChange:
                        this->outputSequencer.ControlChange(nextEvent.channelTrackNumber, nextChannelEvent.value1, nextChannelEvent.value2);
                        break;
                    case ChannelMessageType::NoteOff:
                        this->outputSequencer.NoteOff(nextEvent.channelTrackNumber, nextChannelEvent.value1, nextChannelEvent.value2);
                        break;
                    case ChannelMessageType::NoteOn:
                        this->outputSequencer.NoteOn(nextEvent.channelTrackNumber, nextChannelEvent.value1, nextChannelEvent.value2);
                        break;
                    case ChannelMessageType::ProgramChange:
                        this->outputSequencer.ProgramChange(nextEvent.channelTrackNumber, nextChannelEvent.value1);
                        break;
                }
            }
            break;
            case TrackType::MetaTrack:
            {
                const auto& nextMetaEvent = this->program.MetaTrack()[nextEvent.trackIndex];
                switch(nextMetaEvent.type)
                {
                    case MetaEventType::EndOfTrack:
                        return;
                    case MetaEventType::SetTempo:
                    {
                        uint64 microSecondsPerBeat = (nextMetaEvent.data[0] << 16) | (nextMetaEvent.data[1] << 8) | (nextMetaEvent.data[2]);
                        this->SetTempoInternal({1, microSecondsPerBeat});
                    }
                    break;
                    case MetaEventType::TimeSignature:
                        //ignore
                        break;
                }
            }
            break;
        }

        state.Update(nextEvent);
    }
}
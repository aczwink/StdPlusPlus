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
#include <Std++/Time/TimeMisc.hpp>
#include <Std++/MIDI/EventStreamer.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::MIDI;

//Public methods
void EventScheduler::Schedule(const bool& continuePlaying)
{
	EventStreamer streamer(this->program);

    NextEventResult nextEvent;
    while(continuePlaying and streamer.FindNextEvent(nextEvent))
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
    }
}
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
#include <Std++/MIDI/Program.hpp>
//Local
#include <Std++/Streams/Writers/DataWriter.hpp>
#include <Std++/Multimedia/CodingFormatId.hpp>
#include <Std++/MIDI/EventStreamer.hpp>
#include <Std++/Streams/BufferedOutputStream.hpp>
#include "ProgramLoader.hpp"
#include "Encoder.hpp"
//Namespaces
using namespace StdXX;
using namespace StdXX::MIDI;

//Public methods
void Program::Write(SeekableOutputStream& seekableOutputStream) const
{
    DataWriter fourccWriter(false, seekableOutputStream);
    DataWriter dataWriter(true, seekableOutputStream);

    fourccWriter.WriteUInt32(FOURCC(u8"MThd"));
    dataWriter.WriteUInt32(6);
    dataWriter.WriteUInt16(0);
    dataWriter.WriteUInt16(1);
    dataWriter.WriteUInt16(this->ticksPerBeat);

    this->WriteTrack(seekableOutputStream);
}

//Class functions
Program Program::Load(InputStream &inputStream)
{
    _stdxx_::ProgramLoader programLoader;
    return programLoader.Load(inputStream);
}

//Private methods
#include <StdXXStreams.hpp>
void Program::WriteTrack(SeekableOutputStream& seekableOutputStream) const
{
    DataWriter fourccWriter(false, seekableOutputStream);
    DataWriter dataWriter(true, seekableOutputStream);

    fourccWriter.WriteUInt32(FOURCC(u8"MTrk"));
    uint64 dataOffset = seekableOutputStream.QueryCurrentOffset();
    dataWriter.WriteUInt32(0);

    BufferedOutputStream bufferedOutputStream(seekableOutputStream);
    _stdxx_::Encoder encoder(bufferedOutputStream);

    EventStreamer streamer(*this);
    NextEventResult nextEvent;
    while(streamer.FindNextEvent(nextEvent))
    {
        switch(nextEvent.trackType)
        {
            case TrackType::ChannelTrack:
            {
                const auto& nextChannelEvent = this->GetChannelTrack(nextEvent.channelTrackNumber)[nextEvent.trackIndex];
                encoder.Encode(nextEvent.deltaTime, nextEvent.channelTrackNumber, nextChannelEvent);
            }
            break;
            case TrackType::MetaTrack:
            {
                const auto& nextMetaEvent = this->MetaTrack()[nextEvent.trackIndex];
                encoder.Encode(nextEvent.deltaTime, nextMetaEvent);
            }
            break;
        }
    }
    bufferedOutputStream.Flush();

    uint64 endOffset = seekableOutputStream.QueryCurrentOffset();
    seekableOutputStream.SeekTo(dataOffset);
    dataWriter.WriteUInt32(static_cast<uint32>(endOffset - dataOffset - 4));
}

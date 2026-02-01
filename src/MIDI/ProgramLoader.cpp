/*
 * Copyright (c) 2022-2026 Amir Czwink (amir130@hotmail.de)
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
#include "ProgramLoader.hpp"
#include "Decoder.hpp"
//Local
#include <Std++/Multimedia/CodingFormatId.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Streams/LimitedInputStream.hpp>
#include <Std++/Streams/BufferedInputStream.hpp>
#include <Std++/Containers/Array/StaticArray.hpp>
#include <Std++/MIDI/Event.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::MIDI;

/**
 * MIDI-Spec:
 * https://www.cs.cmu.edu/~music/cmsip/readings/Standard-MIDI-file-format-updated.pdf
 */

//Public methods
Program ProgramLoader::Load(InputStream &inputStream) const
{
    DataReader fourccReader(false, inputStream);
    DataReader dataReader(true, inputStream);

    ASSERT_EQUALS(FOURCC(u8"MThd"), fourccReader.ReadUInt32());
    ASSERT_EQUALS(6, dataReader.ReadUInt32());

    uint16 smfFormat = dataReader.ReadUInt16();
    ASSERT_EQUALS(0, smfFormat);
    uint16 nTracks = dataReader.ReadUInt16();
    uint16 timeDivision = dataReader.ReadUInt16();

    Program program(timeDivision);

    uint16 channelToTrackOffset = 0;
    for(uint16 i = 0; i < nTracks; i++)
    {
        BinaryTreeMap<uint8, uint16> channelMap;
        this->ReadTrack(inputStream, program, channelMap, channelToTrackOffset);
        channelToTrackOffset += channelMap.GetNumberOfElements();
    }

    return program;
}

//Private methods
bool ProgramLoader::IsChannelMessage(SMFEventType eventType) const
{
    switch(eventType)
    {
        case SMFEventType::ChannelPressure:
        case SMFEventType::ControlChange:
        case SMFEventType::NoteOff:
        case SMFEventType::NoteOn:
        case SMFEventType::Pitch:
        case SMFEventType::ProgramChange:
            return true;
    }

    return false;
}

void ProgramLoader::ReadTrack(InputStream &inputStream, Program &program, BinaryTreeMap<uint8, uint16> &channelMap, uint16 channelToTrackOffset) const
{
    DataReader fourccReader(false, inputStream);
    DataReader dataReader(true, inputStream);

    ASSERT_EQUALS(FOURCC(u8"MTrk"), fourccReader.ReadUInt32());
    uint32 dataSize = dataReader.ReadUInt32();

    LimitedInputStream limitedInputStream(inputStream, dataSize);
    BufferedInputStream bufferedInputStream(limitedInputStream);
    Decoder decoder(bufferedInputStream);

    uint64 t = 0;

    bool foundEnd = false;
    SMFEvent event;
    while(!foundEnd)
    {
        decoder.DecodeNextEvent(event);
        t += event.deltaTime;

        uint16 trackNumber = 0;
        if(IsChannelMessage(event.type))
        {
            if(!channelMap.Contains(event.channel))
                channelMap.Insert(event.channel, channelToTrackOffset + event.channel);
            trackNumber = channelMap.Get(event.channel);
        }

        switch(event.type)
        {
            case SMFEventType::ChannelPressure:
                program.AddChannelMessage(ChannelMessageType::ChannelPressure, trackNumber, t, event.value1);
                break;
            case SMFEventType::ControlChange:
                program.AddChannelMessage(ChannelMessageType::ControlChange, trackNumber, t, event.value1, event.value2);
                break;
            case SMFEventType::MetaEvent:
            {
                switch(event.metaType)
                {
                    case 0x2F:
                        foundEnd = true;
                    default:
                        program.AddMetaEvent(static_cast<MetaEventType>(event.metaType), t, event.byteData);
                        break;
                }
            }
            break;
            case SMFEventType::NoteOff:
                program.AddChannelMessage(ChannelMessageType::NoteOff, trackNumber, t, event.value1, event.value2);
                break;
            case SMFEventType::NoteOn:
                program.AddChannelMessage(ChannelMessageType::NoteOn, trackNumber, t, event.value1, event.value2);
                break;
        	case SMFEventType::Pitch:
        		program.AddChannelMessage(ChannelMessageType::Pitch, trackNumber, t, event.value1);
				break;
            case SMFEventType::ProgramChange:
                program.AddChannelMessage(ChannelMessageType::ProgramChange, trackNumber, t, event.value1);
                break;
        }
    }
}
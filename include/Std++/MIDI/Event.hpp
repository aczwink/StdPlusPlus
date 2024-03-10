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
#pragma once

namespace StdXX::MIDI
{
    enum class ChannelMessageType
    {
        ControlChange,
        NoteOff,
        NoteOn,
        ProgramChange
    };

    struct ChannelMessage
    {
        ChannelMessageType type;
        uint64 timeStamp;
        uint8 value1;
        uint8 value2;
    };

    enum class MetaEventType
    {
        EndOfTrack = 0x2f,
        SetTempo = 0x51,
        TimeSignature = 0x58
    };

    struct MetaEvent
    {
        MetaEventType type;
        uint64 timeStamp;
        DynamicByteBuffer data;
    };
}
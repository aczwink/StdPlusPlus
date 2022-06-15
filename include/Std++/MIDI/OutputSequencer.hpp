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
#include <Std++/Definitions.h>

namespace StdXX::MIDI
{
    class OutputSequencer
    {
    public:
        //Abstract
        virtual void ControlChange(uint32 trackNumber, uint8 controllerNumber, uint8 controllerValue) = 0;
        virtual void NoteOff(uint32 trackNumber, uint8 note, uint8 velocity) = 0;
        virtual void NoteOn(uint32 trackNumber, uint8 note, uint8 velocity) = 0;
        virtual void ProgramChange(uint32 trackNumber, uint8 programNumber) = 0;
    };
}
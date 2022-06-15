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
//Class header
#include <Std++/MIDI/MultiConnectionOutputSequencer.hpp>
//Namespaces
using namespace StdXX::MIDI;

void MultiConnectionOutputSequencer::ControlChange(uint32 trackNumber, uint8 controllerNumber, uint8 controllerValue)
{
    uint8 midiChannel;
    StandardOutputSequencer* standardOutputSequencer = this->GetOutputSequencerAndChannelForTrack(trackNumber, midiChannel);
    if(standardOutputSequencer)
        standardOutputSequencer->ControlChange(midiChannel, controllerNumber, controllerValue);
}

void MultiConnectionOutputSequencer::NoteOff(uint32 trackNumber, uint8 note, uint8 velocity)
{
    uint8 midiChannel;
    StandardOutputSequencer* standardOutputSequencer = this->GetOutputSequencerAndChannelForTrack(trackNumber, midiChannel);
    if(standardOutputSequencer)
        standardOutputSequencer->NoteOff(midiChannel, note, velocity);
}

void MultiConnectionOutputSequencer::NoteOn(uint32 trackNumber, uint8 note, uint8 velocity)
{
    uint8 midiChannel;
    StandardOutputSequencer* standardOutputSequencer = this->GetOutputSequencerAndChannelForTrack(trackNumber, midiChannel);
    if(standardOutputSequencer)
        standardOutputSequencer->NoteOn(midiChannel, note, velocity);
}

void MultiConnectionOutputSequencer::ProgramChange(uint32 trackNumber, uint8 programNumber)
{
    uint8 midiChannel;
    StandardOutputSequencer* standardOutputSequencer = this->GetOutputSequencerAndChannelForTrack(trackNumber, midiChannel);
    if(standardOutputSequencer)
        standardOutputSequencer->ProgramChange(midiChannel, programNumber);
}

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
#include "StandardOutputSequencer.hpp"
#include "OutputSequencer.hpp"

namespace StdXX::MIDI
{
    class MultiConnectionOutputSequencer : public OutputSequencer
    {
    public:
        //Methods
        void ControlChange(uint32 trackNumber, uint8 controllerNumber, uint8 controllerValue) override;
        void NoteOff(uint32 trackNumber, uint8 note, uint8 velocity) override;
        void NoteOn(uint32 trackNumber, uint8 note, uint8 velocity) override;
        void ProgramChange(uint32 trackNumber, uint8 programNumber) override;

        //Inline
        inline void Add(StandardOutputSequencer& standardOutputSequencer)
        {
            this->outputSequencers.Push(&standardOutputSequencer);
        }

    private:
        //Variables
        DynamicArray<StandardOutputSequencer*> outputSequencers;

        //Inline
        inline StandardOutputSequencer* GetOutputSequencerAndChannelForTrack(uint32 channelTrackNumber, uint8& midiChannel)
        {
            uint32 index = channelTrackNumber / 16;
            midiChannel = static_cast<uint8>(channelTrackNumber % 16);
            return index < this->outputSequencers.GetNumberOfElements() ? this->outputSequencers[index] : nullptr;
        }
    };
}
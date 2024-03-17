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
//Local
#include <Std++/Math/Rational.hpp>
#include "Program.hpp"
#include "OutputSequencer.hpp"

namespace StdXX::MIDI
{
    class EventScheduler
    {
    public:
        //Constructor
        inline EventScheduler(const Program& program, OutputSequencer& outputSequencer)
            : program(program), outputSequencer(outputSequencer)
        {
            this->SetTempo(120);
        }

        //Methods
        void Schedule(const bool& continuePlaying);

        //Inline
        inline void SetTempo(uint16 beatsPerMinute)
        {
            Math::Rational<uint64> beatsPerMicrosecond(beatsPerMinute, 60_u64 * 1000 * 1000);
            this->SetTempoInternal(beatsPerMicrosecond);
        }

    private:
        //Variables
        const Program& program;
        OutputSequencer& outputSequencer;
        Math::Rational<uint64> ticksPerMicrosecond;

        //Inline
        inline void SetTempoInternal(const Math::Rational<uint64>& beatsPerMicrosecond)
        {
            this->ticksPerMicrosecond = beatsPerMicrosecond * this->program.TicksPerBeat();
        }
    };
}
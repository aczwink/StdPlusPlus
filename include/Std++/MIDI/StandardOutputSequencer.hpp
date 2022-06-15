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
#include <Std++/Debug.hpp>

namespace StdXX::MIDI
{
    class StandardOutputSequencer
    {
    public:
        //Abstract
        virtual void SendMessage(const uint8* message, uint8 messageSize) = 0;

        //Inline
        inline void ControlChange(uint8 channel, uint8 controllerNumber, uint8 controllerValue)
        {
            ASSERT(channel < 16, u8"Channel is out of range");
            ASSERT(controllerNumber < 120, u8"Controller number is out of range");
            ASSERT(controllerValue < 128, u8"Controller value is out of range");

            uint8 message[3];
            message[0] = static_cast<uint8>(0xB0 | channel);
            message[1] = controllerNumber;
            message[2] = controllerValue;

            this->SendMessage(message, sizeof(message));
        }

        inline void NoteOff(uint8 channel, uint8 note, uint8 velocity)
        {
            ASSERT(channel < 16, u8"Channel is out of range");
            ASSERT(note < 128, u8"Note is out of range");
            ASSERT(velocity < 128, u8"Velocity is out of range");

            uint8 message[3];
            message[0] = static_cast<uint8>(0x80 | channel);
            message[1] = note;
            message[2] = velocity;

            this->SendMessage(message, sizeof(message));
        }

        inline void NoteOn(uint8 channel, uint8 note, uint8 velocity)
        {
            ASSERT(channel < 16, u8"Channel is out of range");
            ASSERT(note < 128, u8"Note is out of range");
            ASSERT(velocity < 128, u8"Velocity is out of range");

            uint8 message[3];
            message[0] = static_cast<uint8>(0x90 | channel);
            message[1] = note;
            message[2] = velocity;

            this->SendMessage(message, sizeof(message));
        }

        inline void ProgramChange(uint8 channel, uint8 programNumber)
        {
            ASSERT(channel < 16, u8"Channel is out of range");
            ASSERT(programNumber < 128, u8"Program number is out of range");

            uint8 message[2];
            message[0] = static_cast<uint8>(0xC0 | channel);
            message[1] = programNumber;

            this->SendMessage(message, sizeof(message));
        }
    };
}
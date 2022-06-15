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
//Global
#include <alsa/asoundlib.h>
//Local
#include <Std++/MIDI/StandardOutputSequencer.hpp>

namespace _stdxx_
{
    class ALSAOutputSequencer : public StdXX::MIDI::StandardOutputSequencer
    {
    public:
        //Constructor
        ALSAOutputSequencer(uint8 clientNumber, uint8 portNumber);

        //Destructor
        ~ALSAOutputSequencer();

        //Methods
        void SendMessage(const uint8 *message, uint8 messageSize) override;

    private:
        //Variables
        snd_seq_t *seq;
        int applicationPort;
        snd_seq_port_subscribe_t *subscription;
        uint32 bufferSize;
        snd_midi_event_t* midiEvent;
    };
}
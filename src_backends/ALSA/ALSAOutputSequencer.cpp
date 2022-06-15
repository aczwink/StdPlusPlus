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
#include "ALSAOutputSequencer.hpp"
//Local
#include <Std++/Debug.hpp>
#include <Std++/Memory.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Constructor
ALSAOutputSequencer::ALSAOutputSequencer(uint8 clientNumber, uint8 portNumber)
{
    snd_seq_addr_t receiver;
    receiver.client = clientNumber;
    receiver.port = portNumber;

    int result = snd_seq_open(&this->seq, "default", SND_SEQ_OPEN_OUTPUT, 0);
    ASSERT_EQUALS(0, result);

    this->applicationPort = snd_seq_create_simple_port( this->seq, "StdXX-StandardOutputSequencer-ApplicationPort",
                                                        SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
                                                        SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION);

    snd_seq_addr_t sender;
    sender.client = snd_seq_client_id( this->seq );
    sender.port = this->applicationPort;

    result = snd_seq_port_subscribe_malloc( &this->subscription );
    ASSERT_EQUALS(0, result);
    snd_seq_port_subscribe_set_sender( this->subscription, &sender );
    snd_seq_port_subscribe_set_dest( this->subscription, &receiver );
    snd_seq_port_subscribe_set_time_update( this->subscription, 1 );
    snd_seq_port_subscribe_set_time_real( this->subscription, 1 );
    result = snd_seq_subscribe_port( this->seq, this->subscription );
    ASSERT_EQUALS(0, result);

    this->bufferSize = 32;

    result = snd_midi_event_new( this->bufferSize, &this->midiEvent );
    ASSERT_EQUALS(0, result);
    snd_midi_event_init( this->midiEvent );
}

//Destructor
ALSAOutputSequencer::~ALSAOutputSequencer()
{
    int result = snd_seq_unsubscribe_port( this->seq, this->subscription );
    ASSERT_EQUALS(0, result);
    snd_seq_port_subscribe_free( this->subscription );

    result = snd_seq_delete_port(this->seq, this->applicationPort);
    ASSERT_EQUALS(0, result);

    result = snd_seq_close(this->seq);
    ASSERT_EQUALS(0, result);
}

//Public methods
void ALSAOutputSequencer::SendMessage(const uint8 *message, uint8 messageSize)
{
    ASSERT(messageSize <= this->bufferSize, u8"TODO: implement this correctly");

    uint8 offset = 0;
    while (offset < messageSize)
    {
        snd_seq_event_t ev;
        snd_seq_ev_clear( &ev );
        snd_seq_ev_set_source( &ev, this->applicationPort );
        snd_seq_ev_set_subs( &ev );
        snd_seq_ev_set_direct( &ev );
        long result = snd_midi_event_encode( this->midiEvent, message + offset, messageSize - offset, &ev );
        if ( result < 0 )
            NOT_IMPLEMENTED_ERROR; //TODO: implement me

        if ( ev.type == SND_SEQ_EVENT_NONE )
            NOT_IMPLEMENTED_ERROR; //TODO: implement me

        offset += result;

        result = snd_seq_event_output( this->seq, &ev );
        if(result < 0)
            NOT_IMPLEMENTED_ERROR; //TODO: implement me
    }
    snd_seq_drain_output( this->seq );
}

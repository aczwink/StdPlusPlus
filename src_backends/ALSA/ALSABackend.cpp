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
#include "ALSABackend.hpp"
//Global
#include <alsa/asoundlib.h>
//Local
#include "ALSAOutputSequencer.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Public methods
UniquePointer<MIDI::StandardOutputSequencer> ALSABackend::ConnectToOutput(const String& portId) const
{
    bool portExists = false;
    for(const auto& port : this->QueryPorts())
    {
        if(port.id == portId)
        {
            portExists = true;
            break;
        }
    }
    if(not portExists)
        NOT_IMPLEMENTED_ERROR; //TODO: implement me

    auto parts = portId.Split(u8":");
    ASSERT_EQUALS(2, parts.GetNumberOfElements());

    return new ALSAOutputSequencer((uint8)parts[0].ToUInt32(), (uint8)parts[1].ToUInt32());
}

DynamicArray<MIDI::Port> ALSABackend::QueryPorts() const
{
    DynamicArray<MIDI::Port> ports;

    snd_seq_t *seq;

    int result = snd_seq_open(&seq, "default", SND_SEQ_OPEN_OUTPUT, 0);
    ASSERT_EQUALS(0, result);

    snd_seq_client_info_t *clientInfo;

    result = snd_seq_client_info_malloc(&clientInfo);
    ASSERT_EQUALS(0, result);

    snd_seq_client_info_set_client( clientInfo, -1 );
    while ( snd_seq_query_next_client( seq, clientInfo ) >= 0 )
    {
        snd_seq_port_info_t *portInfo;
        result = snd_seq_port_info_malloc(&portInfo);
        ASSERT_EQUALS(0, result);

        int client = snd_seq_client_info_get_client( clientInfo );
        snd_seq_port_info_set_client( portInfo, client );
        snd_seq_port_info_set_port( portInfo, -1 );

        while ( snd_seq_query_next_port( seq, portInfo ) >= 0 )
        {
            unsigned int caps = snd_seq_port_info_get_capability( portInfo );
            if(caps & SND_SEQ_PORT_CAP_WRITE)
            {
                MIDI::Port port;

                port.id = String::Number(snd_seq_port_info_get_client( portInfo ))
                        + u8":"
                        + String::Number(snd_seq_port_info_get_port(portInfo));
                port.name = String::CopyRawString(snd_seq_client_info_get_name(clientInfo))
                        + String(u8":")
                        + snd_seq_port_info_get_name(portInfo);

                ports.Push(port);
            }
        }

        snd_seq_port_info_free(portInfo);
    }

    snd_seq_client_info_free(clientInfo);

    result = snd_seq_close(seq);
    ASSERT_EQUALS(0, result);

    return ports;
}
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
#include "Decoder.hpp"
//Local
#include <Std++/Streams/Readers/DataReader.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Private methods
uint8 Decoder::DecodeEvent(InputStream& inputStream, SMFEvent& event, uint8 lastStatusByte)
{
    DataReader dataReader(true, inputStream);

    event.deltaTime = Decoder::DecodeVariableLengthInteger(dataReader);
    uint8 statusByte = dataReader.ReadByte();

    if((statusByte & 0xF0) == 0xF0)
    {
        switch(statusByte)
        {
            case 0xFF:
            {
                event.type = SMFEventType::MetaEvent;
                event.metaType = dataReader.ReadByte();

                uint32 length = Decoder::DecodeVariableLengthInteger(dataReader);
                event.byteData.Resize(length);
                dataReader.ReadBytes(event.byteData.Data(), event.byteData.Size());
            }
            break;
            default:
                NOT_IMPLEMENTED_ERROR; //TODO: implement me
        }
    }
    else
    {
        if((statusByte & 0x80) == 0)
        {
            //running status
            event.value1 = statusByte;
            statusByte = lastStatusByte;
        }
        else
            event.value1 = dataReader.ReadByte();

        event.channel = static_cast<uint8>(statusByte & 0xF);

        switch(statusByte & 0xF0)
        {
            case 0x80:
            {
                event.type = SMFEventType::NoteOff;
                event.value2 = dataReader.ReadByte();
            }
            break;
            case 0x90:
            {
                event.type = SMFEventType::NoteOn;
                event.value2 = dataReader.ReadByte();
            }
            break;
            case 0xB0:
            {
                event.type = SMFEventType::ControlChange;
                event.value2 = dataReader.ReadByte();
            }
            break;
            case 0xC0:
                event.type = SMFEventType::ProgramChange;
            break;
            case 0xD0:
                event.type = SMFEventType::ChannelPressure;
            break;
        	case 0xE0:
			{
				event.type = SMFEventType::Pitch;
				event.value1 = (event.value1 & 0x7F_u16) | ((dataReader.ReadByte() & 0x7F_u16) << 7);
			}
			break;
            default:
                NOT_IMPLEMENTED_ERROR; //TODO: implement me
        }
    }

    return statusByte;
}

uint32 Decoder::DecodeVariableLengthInteger(DataReader& dataReader)
{
    uint32 result = 0;
    uint8 nextByte;
    do
    {
        nextByte = dataReader.ReadByte();
        result = (result << 7) | (nextByte & 0x7F);
    }
    while(nextByte & 0x80);

    return result;
}

/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
#include "Encoder.hpp"
//Local
#include <Std++/Streams/Writers/DataWriter.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::MIDI;

//Public methods
void Encoder::Encode(uint32 deltaTime, uint16 channel, const ChannelMessage &message)
{
	DataWriter dataWriter(true, this->outputStream);

	this->EncodeVariableLengthInteger(deltaTime);

	uint8 statusByte;
	switch(message.type)
	{
		case ChannelMessageType::ControlChange:
			statusByte = 0xB0;
			break;
		case ChannelMessageType::NoteOff:
			statusByte = 0x80;
			break;
		case ChannelMessageType::NoteOn:
			statusByte = 0x90;
			break;
		case ChannelMessageType::ProgramChange:
			statusByte = 0xC0;
			break;
	}
	statusByte |= channel;

	if(statusByte != this->lastStatusByte)
		dataWriter.WriteByte(statusByte);
	this->lastStatusByte = statusByte;
	dataWriter.WriteByte(message.value1);

	if(message.type != ChannelMessageType::ProgramChange)
		dataWriter.WriteByte(message.value2);
}

void Encoder::Encode(uint32 deltaTime, const MetaEvent& metaEvent)
{
	DataWriter dataWriter(true, this->outputStream);

	this->EncodeVariableLengthInteger(deltaTime);

	dataWriter.WriteByte(0xFF);
	this->lastStatusByte = 0xFF;
	dataWriter.WriteByte(static_cast<uint8>(metaEvent.type));

	this->EncodeVariableLengthInteger(metaEvent.data.Size());
	dataWriter.WriteBytes(metaEvent.data.Data(), metaEvent.data.Size());
}

//Private methods
void Encoder::EncodeVariableLengthInteger(uint32 value)
{
	DataWriter dataWriter(true, this->outputStream);

	uint8 buffer[5];
	uint8 index = 0;

	do
	{
		buffer[index++] = static_cast<uint8>(value & 0x7F);
		value >>= 7;
	}
	while(value);

	//index--;
	for(uint8 i = 0; i < index; i++)
	{
		if((i+1) == index)
			dataWriter.WriteByte(buffer[index - i - 1]);
		else
			dataWriter.WriteByte(0x80_u8 | buffer[index - i - 1]);
	}
}
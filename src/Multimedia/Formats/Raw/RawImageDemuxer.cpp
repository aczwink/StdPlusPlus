/*
 * Copyright (c) 2018-2023 Amir Czwink (amir130@hotmail.de)
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
#include "RawImageDemuxer.hpp"
//Local
#include "RawImageFormat.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Public methods
void RawImageDemuxer::ReadHeader()
{
	Stream *stream = new Stream(DataType::Video);
	stream->SetCodingFormat(this->codingFormatId);

	this->AddStream(stream);
}

UniquePointer<IPacket> RawImageDemuxer::ReadPacket()
{
	if(this->inputStream.IsAtEnd())
		return nullptr;

	UniquePointer<Packet> packet = new Packet(this->inputStream.QueryRemainingBytes());
	packet->streamIndex = 0;

	this->inputStream.ReadBytes(packet->GetData(), packet->GetSize());

	return Move(packet);
}

void RawImageDemuxer::Seek(uint64 timestamp, const class TimeScale &timeScale)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

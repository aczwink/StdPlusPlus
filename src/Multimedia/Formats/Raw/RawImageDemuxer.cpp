/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/VideoStream.hpp>
#include "RawImageFormat.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructor
RawImageDemuxer::RawImageDemuxer(const Format &format, SeekableInputStream &inputStream, CodecId id) : Demuxer(format, inputStream)
{
	this->codecId = codecId;
}

//Public methods
void RawImageDemuxer::ReadHeader()
{
	Stream *stream = new VideoStream;
	stream->SetCodec(Codec::GetCodec(this->codecId));

	this->AddStream(stream);
}

bool RawImageDemuxer::ReadPacket(Packet &packet)
{
	if(this->inputStream.IsAtEnd())
		return false;

	packet.Allocate(this->inputStream.GetRemainingBytes());
	packet.streamIndex = 0;

	this->inputStream.ReadBytes(packet.GetData(), packet.GetSize());

	return true;
}

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
#include "libavcodec_ParserContext.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructor
libavcodec_ParserContext::libavcodec_ParserContext(AVCodecID libavCodecId)
{
	this->codecContext = avcodec_alloc_context3(avcodec_find_decoder(libavCodecId));
	this->parserContext = av_parser_init(libavCodecId);
	this->packet = av_packet_alloc();
}

//Destructor
libavcodec_ParserContext::~libavcodec_ParserContext()
{
	av_packet_free(&this->packet);
	av_parser_close(this->parserContext);
	avcodec_free_context(&this->codecContext);
}

//Public methods
void libavcodec_ParserContext::Parse(const Packet &packet)
{
	const byte *data = packet.GetData();
	uint32 leftSize = packet.GetSize();

	//parse packet into frame packets
	while (leftSize)
	{
		//parse frames from our packet
		int ret = av_parser_parse2(this->parserContext, this->codecContext, &this->packet->data, &this->packet->size, data, leftSize, packet.pts, AV_NOPTS_VALUE, 0);
		if (ret < 0)
			break; //an error occured. skip packet

		data += ret;
		leftSize -= ret;

		if (this->packet->size)
			this->MapPacket(packet);
	}
}

//Private methods
void libavcodec_ParserContext::MapPacket(const Packet &sourcePacket)
{
	Packet p;

	p.Allocate(this->packet->size);
	MemCopy(p.GetData(), this->packet->data, p.GetSize());

	if ((this->parserContext->key_frame == 1) || (this->parserContext->key_frame == -1 && sourcePacket.containsKeyframe))
		p.containsKeyframe = true;
	p.pts = this->parserContext->pts;
	p.streamIndex = sourcePacket.streamIndex;

	this->AddToFrameBuffer(p);
	this->ReadyFrameBuffer();
}
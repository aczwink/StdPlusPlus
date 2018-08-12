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
#include "libavcodec_DecoderContext.hpp"
//Local
#include <Std++/Multimedia/VideoStream.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructor
libavcodec_DecoderContext::libavcodec_DecoderContext(const Decoder &decoder, Stream &stream, AVCodec *codec) : DecoderContext(decoder)
{
	this->codecContext = avcodec_alloc_context3(codec);
	this->packet = av_packet_alloc();
	this->frame = av_frame_alloc();

	switch (stream.GetType())
	{
	case DataType::Video:
	{
		VideoStream &videoStream = (VideoStream &)stream;

		//some codecs don't have this info in its encoded data
		if (videoStream.size.width)
			this->codecContext->width = videoStream.size.width;
		if (videoStream.size.height)
			this->codecContext->height = videoStream.size.height;
	}
	break;
	}

	int ret = avcodec_open2(this->codecContext, codec, nullptr);
	ASSERT(ret == 0, u8"TODO: not implemented on failure...");
}

//Destructor
libavcodec_DecoderContext::~libavcodec_DecoderContext()
{
	av_frame_free(&this->frame);
	av_packet_free(&this->packet);
	avcodec_free_context(&this->codecContext);
}

//Public methods
void libavcodec_DecoderContext::Decode(const Packet & packet)
{
	this->MapPacket(packet);

	int ret = avcodec_send_packet(this->codecContext, this->packet);
	ASSERT(ret == 0, u8"TODO: implement error handling");
	/*if (ret < 0)
		return; //an error occured... skip packet*/

	while (ret >= 0)
	{
		ret = avcodec_receive_frame(this->codecContext, this->frame);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			return;
		ASSERT(ret == 0, u8"TODO: implement error handling");
		/*else if (ret < 0)
			return; //an error occured. skip packet*/
					
		//ready frame
		switch (this->codecContext->codec_type)
		{
		case AVMEDIA_TYPE_AUDIO:
		{
			NOT_IMPLEMENTED_ERROR; //TODO: next lines
			/*
			switch (this->frame->format)
			{
			case AV_SAMPLE_FMT_S16P:
			{
				AudioBuffer<int16> *buffer = new AudioBuffer<int16>(MapChannels(state.frame->channels), (uint32)state.frame->nb_samples);
				for (uint32 i = 0; i < state.frame->channels; i++)
				{
					MemCopy(buffer->GetChannel((Channel)i), state.frame->data[i], state.frame->nb_samples * sizeof(int16));
				}
				Frame *frame = new AudioFrame(buffer);
				CopyImportantInfo(*state.frame, *frame);
				frames.Push(frame);
			}
			break;
			default:
				NOT_IMPLEMENTED_ERROR;
			}*/
		}
		break;
		case AVMEDIA_TYPE_VIDEO:
			this->MapVideoFrame();
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	}
}

//Private methods
void libavcodec_DecoderContext::MapPacket(const StdXX::Multimedia::Packet &packet)
{
	this->packet->data = (uint8_t *)packet.GetData();
	this->packet->size = packet.GetSize();
}

void libavcodec_DecoderContext::MapVideoFrame()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	/*switch (state.frame->format)
	{
	case AV_PIX_FMT_YUV420P:
	{
	YCbCr420Image *image = new YCbCr420Image(state.frame->width, state.frame->height, false);
	for (uint32 i = 0; i < state.frame->height; i++)
	MemCopy(&image->GetLumaChannel()[i * state.frame->width], &state.frame->data[0][i * state.frame->linesize[0]], state.frame->width);
	for (uint32 i = 0; i < state.frame->height / 2; i++)
	{
	MemCopy(&image->GetChromaRedChannel()[i * state.frame->width / 2], &state.frame->data[1][i * state.frame->linesize[1]], state.frame->width / 2);
	MemCopy(&image->GetChromaBlueChannel()[i * state.frame->width / 2], &state.frame->data[2][i * state.frame->linesize[2]], state.frame->width / 2);
	}
	Frame *frame = new VideoFrame(image);
	CopyImportantInfo(*state.frame, *frame);
	frames.Push(frame);
	}
	break;
	default:
	NOT_IMPLEMENTED_ERROR;
	}*/
}
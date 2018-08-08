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
#include "libavcodec_Encoder.hpp"
//Local
#include "../libavcodec_Backend.hpp"
#include <Std++/Debug.hpp>
#include <Std++/Multimedia/VideoFrame.hpp>
#include <Std++/_Backends/BackendManager.hpp>
#include <Std++/Multimedia/VideoStream.hpp>
#include <Std++/Multimedia/Pixmaps/YCbCr/YCbCr420Image.hpp>
#include <Std++/Multimedia/Pixmaps/RGB/R8G8B8Image.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructor
libavcodec_Encoder::libavcodec_Encoder(Stream &stream, CodingFormatId codecId)
{
	NOT_IMPLEMENTED_ERROR; //TODO: next line
	//this->codec = avcodec_find_encoder(MapCodecId(codecId));
	this->codecContext = avcodec_alloc_context3(this->codec);
	this->packet = av_packet_alloc();
	this->frame = av_frame_alloc();

	this->codecContext->time_base.num = stream.timeScale.numerator;
	this->codecContext->time_base.den = stream.timeScale.denominator;
	switch(stream.GetType())
	{
		case DataType::Audio:
			break;
		case DataType::Video:
		{
			const VideoStream &videoStream = (const VideoStream &) stream;

			this->codecContext->pix_fmt = *this->codec->pix_fmts; //lets just get the first one which is supported
			this->codecContext->width = videoStream.width;
			this->codecContext->height = videoStream.height;
		}
		break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	int ret = avcodec_open2(this->codecContext, this->codec, nullptr);
	ASSERT(ret == 0, u8"TODO: implement this curroctly");
}

//Destructor
libavcodec_Encoder::~libavcodec_Encoder()
{
	av_frame_free(&this->frame);
	av_packet_free(&this->packet);
	avcodec_free_context(&this->codecContext);
}

//Public methods
void libavcodec_Encoder::Encode(const Frame &frame)
{
	this->MapFrame(frame);
	this->Encode(this->frame);
}

void libavcodec_Encoder::Flush()
{
	this->Encode(nullptr);
}

//Private methods
void libavcodec_Encoder::Encode(AVFrame *frame)
{
	int ret = avcodec_send_frame(this->codecContext, frame);
	ASSERT(ret >= 0, u8"TODO: implement this correctly");

	while(ret >= 0)
	{
		ret = avcodec_receive_packet(this->codecContext, this->packet);
		if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) //need more frames
			return;
		ASSERT(ret == 0, u8"TODO: implement this correctly")

		this->MapPacket();

		av_packet_unref(this->packet);
	}
}

void libavcodec_Encoder::MapFrame(const Frame &frame) const
{
	const VideoFrame &videoFrame = (const VideoFrame &) frame;

	av_frame_unref(this->frame);

	this->frame->format = this->codecContext->pix_fmt;
	this->frame->width = videoFrame.GetPixmap()->GetWidth();
	this->frame->height = videoFrame.GetPixmap()->GetHeight();
	this->frame->pts = frame.pts;

	int ret = av_frame_get_buffer(this->frame, 0);
	ASSERT(ret == 0, u8"TODO: implement this correctly");

	ret = av_frame_make_writable(this->frame);
	ASSERT(ret == 0, u8"TODO: implement this correctly");

	const Pixmap *pixmap = videoFrame.GetPixmap();

	switch(this->frame->format)
	{
		case AV_PIX_FMT_RGB24:
		{
			R8G8B8Image *image = (R8G8B8Image *) pixmap;
			for(uint32 line = 0; line < image->GetHeight(); line++)
			{
				for(uint32 x = 0; x < image->GetWidth(); x++)
				{
					uint32 baseIdx = (image->GetHeight() - line - 1) * this->frame->linesize[0] + x*3;
					uint32 rightIdx = line * image->GetWidth() + x;

					this->frame->data[0][baseIdx+0] = image->GetRedChannel()[rightIdx];
					this->frame->data[0][baseIdx+1] = image->GetGreenChannel()[rightIdx];
					this->frame->data[0][baseIdx+2] = image->GetBlueChannel()[rightIdx];
				}
			}
		}
		break;
		case AV_PIX_FMT_YUV420P:
		{
			YCbCr420Image *yCbCr420Image = (YCbCr420Image *) pixmap;
			uint32 line;
			for(line = 0; line < yCbCr420Image->GetHeight() / 2; line++)
			{
				uint32 invLine = yCbCr420Image->GetHeight() - line - 1;
				MemCopy(&this->frame->data[0][invLine * this->frame->linesize[0]], &yCbCr420Image->GetLumaChannel()[line * yCbCr420Image->GetWidth()], yCbCr420Image->GetWidth());

				invLine = yCbCr420Image->GetHeight()/2 - line - 1;
				MemCopy(&this->frame->data[1][invLine * this->frame->linesize[1]], &yCbCr420Image->GetChromaBlueChannel()[line * yCbCr420Image->GetWidth() / 2], yCbCr420Image->GetWidth() / 2);
				MemCopy(&this->frame->data[2][invLine * this->frame->linesize[2]], &yCbCr420Image->GetChromaRedChannel()[line * yCbCr420Image->GetWidth() / 2], yCbCr420Image->GetWidth() / 2);
			}
			for(; line < yCbCr420Image->GetHeight(); line++)
			{
				uint32 invLine = yCbCr420Image->GetHeight() - line - 1;
				MemCopy(&this->frame->data[0][invLine * this->frame->linesize[0]], &yCbCr420Image->GetLumaChannel()[line * yCbCr420Image->GetWidth()], yCbCr420Image->GetWidth());
			}
		}
		break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: write everything
	}
}

void libavcodec_Encoder::MapPacket()
{
	Packet *packet = new Packet;

	//data
	packet->Allocate(static_cast<uint32>(this->packet->size));
	MemCopy(packet->GetData(), this->packet->data, packet->GetSize());

	//meta
	packet->pts = this->packet->pts;
	packet->containsKeyframe = (this->packet->flags & AV_PKT_FLAG_KEY) != 0;

	this->AddPacket(packet);
}

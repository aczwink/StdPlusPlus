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
#include "libavcodec_EncoderContext.hpp"
//Local
#include "../libavcodec_Backend.hpp"
#include <Std++/Debug.hpp>
#include <Std++/Multimedia/VideoFrame.hpp>
#include <Std++/_Backends/BackendManager.hpp>
#include <Std++/Multimedia/VideoStream.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructor
libavcodec_EncoderContext::libavcodec_EncoderContext(Stream &stream, AVCodec *codec, const BijectiveMap<NamedPixelFormat, AVPixelFormat> &libavPixelFormatMap)
		: libavPixelFormatMap(libavPixelFormatMap)
{
	this->codecContext = avcodec_alloc_context3(codec);
	this->packet = av_packet_alloc();
	this->frame = av_frame_alloc();

	this->codecContext->time_base.num = static_cast<int>(stream.timeScale.numerator);
	this->codecContext->time_base.den = static_cast<int>(stream.timeScale.denominator);
	switch(stream.GetType())
	{
		case DataType::Audio:
			break;
		case DataType::Video:
		{
			const VideoStream &videoStream = (const VideoStream &) stream;

			ASSERT(!videoStream.pixelFormat.IsNull(), u8"You must give a pixel format.");
			ASSERT(videoStream.pixelFormat->GetNameIfExisting(this->namedPixelFormat), u8"TODO: ...");

			this->codecContext->pix_fmt = this->libavPixelFormatMap.Get(this->namedPixelFormat);
			this->codecContext->width = videoStream.size.width;
			this->codecContext->height = videoStream.size.height;
		}
		break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	int ret = avcodec_open2(this->codecContext, codec, nullptr);
	ASSERT(ret == 0, u8"TODO: implement this curroctly");
}

//Destructor
libavcodec_EncoderContext::~libavcodec_EncoderContext()
{
	av_frame_free(&this->frame);
	av_packet_free(&this->packet);
	avcodec_free_context(&this->codecContext);
}

//Public methods
void libavcodec_EncoderContext::Encode(const Frame &frame)
{
	this->MapFrame(frame);
	this->Encode(this->frame);
}

void libavcodec_EncoderContext::Flush()
{
	this->Encode(nullptr);
}

//Private methods
void libavcodec_EncoderContext::Encode(AVFrame *frame)
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

void libavcodec_EncoderContext::MapFrame(const Frame &frame) const
{
	const VideoFrame &videoFrame = (const VideoFrame &) frame;

	NamedPixelFormat framePixelFormat;
	ASSERT(videoFrame.GetPixmap()->GetPixelFormat().GetNameIfExisting(framePixelFormat), u8"TODO: ...");
	ASSERT(this->namedPixelFormat == framePixelFormat, u8"Pixel format changed? Check what libavcodec would say to that...");

	av_frame_unref(this->frame);

	this->frame->format = this->libavPixelFormatMap.Get(framePixelFormat);
	this->frame->width = videoFrame.GetPixmap()->GetSize().width;
	this->frame->height = videoFrame.GetPixmap()->GetSize().height;
	this->frame->pts = frame.pts;

	int ret = av_frame_get_buffer(this->frame, 0);
	ASSERT(ret == 0, u8"TODO: implement this correctly");

	ret = av_frame_make_writable(this->frame);
	ASSERT(ret == 0, u8"TODO: implement this correctly");

	const Pixmap *pixmap = videoFrame.GetPixmap();
	const PixelFormat &pixelFormat = pixmap->GetPixelFormat();

	for(uint8 i = 0; i < pixelFormat.nPlanes; i++)
	{
		for(uint32 line = 0; line < pixmap->GetNumberOfLines(i); line++)
		{
			uint32 libav_lineIndex = (pixmap->GetNumberOfLines(i) - line - 1) * this->frame->linesize[i];
			const void *src = ((byte *)pixmap->GetPlane(i)) + line * pixmap->GetLineSize(i);
			MemCopy(&this->frame->data[i][libav_lineIndex], src, Math::Min((uint32)this->frame->linesize[i], pixmap->GetLineSize(i))); //min size because of alignment
		}
	}
}

void libavcodec_EncoderContext::MapPacket()
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

/*
 * Copyright (c) 2018-2024 Amir Czwink (amir130@hotmail.de)
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
#include "../libavcodec_Extension.hpp"
#include <Std++/Debug.hpp>
#include <Std++/_Backends/BackendManager.hpp>
#include <Std++/_Backends/ExtensionManager.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructor
libavcodec_EncoderContext::libavcodec_EncoderContext(const EncodingParameters& encodingParameters, const AVCodec *codec) : EncoderContext(encodingParameters)
{
	const auto& codingParameters = encodingParameters;

	this->codecContext = avcodec_alloc_context3(codec);
	this->packet = av_packet_alloc();
	this->frame = av_frame_alloc();

	this->codecContext->time_base.num = static_cast<int>(encodingParameters.timeScale.numerator);
	this->codecContext->time_base.den = static_cast<int>(encodingParameters.timeScale.denominator);
	switch(codingParameters.dataType)
	{
		case DataType::Audio:
		{

			ASSERT(codingParameters.audio.sampleFormat.HasValue(), u8"You must give a sample format.");
			switch (codingParameters.audio.sampleFormat->sampleType)
			{
				case AudioSampleType::S16:
				{
					if (codingParameters.audio.sampleFormat->IsPlanar())
						this->codecContext->sample_fmt = AV_SAMPLE_FMT_S16P;
					else
						this->codecContext->sample_fmt = AV_SAMPLE_FMT_S16;
				}
				break;
				case AudioSampleType::U8:
				{
					if (codingParameters.audio.sampleFormat->IsPlanar())
						this->codecContext->sample_fmt = AV_SAMPLE_FMT_U8P;
					else
						this->codecContext->sample_fmt = AV_SAMPLE_FMT_U8;
				}
				break;
				default:
					NOT_IMPLEMENTED_ERROR;
			}
			
			//AV_SAMPLE_FMT_FLTP

			this->codecContext->sample_rate = codingParameters.audio.sampleRate;
			this->codecContext->ch_layout = this->MapChannelLayout(*codingParameters.audio.sampleFormat);
		}
		break;
		case DataType::Video:
		{
			ASSERT(codingParameters.video.pixelFormat.HasValue(), u8"You must give a pixel format.");

			bool nameFound = codingParameters.video.pixelFormat.Value().GetNameIfExisting(this->namedPixelFormat);
			ASSERT(nameFound, u8"TODO: ...");

			this->codecContext->pix_fmt = ExtensionManager::GetRootInstance().GetExtension<libavcodec_Extension>()->MapPixelFormat(this->namedPixelFormat);
			this->codecContext->width = codingParameters.video.size.width;
			this->codecContext->height = codingParameters.video.size.height;
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
	switch (frame.GetType())
	{
	case DataType::Audio:
		this->MapAudioFrame(frame);
		break;
	case DataType::Subtitle:
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	case DataType::Video:
		this->MapVideoFrame(frame);
		break;
	}
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

bool libavcodec_EncoderContext::FindSampleFormat(AVSampleFormat sampleFormat, const AVCodec *codec) const
{
	ASSERT(codec->sample_fmts, u8"Report this please!");
	const AVSampleFormat *current = codec->sample_fmts;

	while ((*current != -1))
	{
		if (*current == sampleFormat)
			return true;
		current++;
	}
	return false;
}

void libavcodec_EncoderContext::MapAudioFrame(const Frame &audioFrame) const
{
	const AudioBuffer *audioBuffer = audioFrame.GetAudioBuffer();

	av_frame_unref(this->frame);

	this->frame->format = this->codecContext->sample_fmt;
	this->frame->pts = audioFrame.pts;
	this->frame->nb_samples = audioBuffer->GetNumberOfSamplesPerChannel();
	this->frame->ch_layout = this->MapChannelLayout(*this->Parameters().audio.sampleFormat);

	int ret = av_frame_get_buffer(this->frame, 0);
	ASSERT(ret == 0, u8"TODO: implement this correctly");

	ret = av_frame_make_writable(this->frame);
	ASSERT(ret == 0, u8"TODO: implement this correctly");
	
	for (uint8 i = 0; i < this->Parameters().audio.sampleFormat->nPlanes; i++)
	{
		MemCopy(this->frame->data[i], audioBuffer->GetPlane(i), Math::Min((uint32)this->frame->linesize[i], audioBuffer->GetPlaneSize(i))); //min size because of alignment
	}
}

AVChannelLayout libavcodec_EncoderContext::MapChannelLayout(const AudioSampleFormat &sampleFormat) const
{
	if(
		(sampleFormat.nChannels == 1)
	)
	{
		AVChannelLayout channelLayout;
		av_channel_layout_default(&channelLayout, 1);
		return channelLayout;
	}
	NOT_IMPLEMENTED_ERROR; //TODO: implement this correctly for all cases
}

void libavcodec_EncoderContext::MapPacket()
{
	Packet *packet = new Packet;

	//data
	packet->Allocate(static_cast<uint32>(this->packet->size));
	MemCopy(packet->GetData(), this->packet->data, packet->GetSize());

	//meta
	if (this->packet->pts == AV_NOPTS_VALUE)
		packet->pts = Unsigned<uint64>::Max();
	else
		packet->pts = this->packet->pts;
	packet->containsKeyframe = (this->packet->flags & AV_PKT_FLAG_KEY) != 0;

	this->AddPacket(packet);
}

void libavcodec_EncoderContext::MapVideoFrame(const Frame &videoFrame) const
{
	NamedPixelFormat framePixelFormat;
	ASSERT(videoFrame.GetPixmap()->GetPixelFormat().GetNameIfExisting(framePixelFormat), u8"TODO: ...");
	ASSERT(this->namedPixelFormat == framePixelFormat, u8"Pixel format changed? Check what libavcodec would say to that...");

	av_frame_unref(this->frame);

	this->frame->format = this->codecContext->pix_fmt;
	this->frame->width = videoFrame.GetPixmap()->GetSize().width;
	this->frame->height = videoFrame.GetPixmap()->GetSize().height;
	this->frame->pts = videoFrame.pts;

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
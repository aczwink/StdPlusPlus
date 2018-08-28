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
#include <Std++/Multimedia/AudioBuffer.hpp>
#include <Std++/Multimedia/AudioFrame.hpp>
#include <Std++/Multimedia/AudioStream.hpp>
#include <Std++/Multimedia/Pixmap.hpp>
#include <Std++/Multimedia/VideoFrame.hpp>
#include <Std++/Multimedia/VideoStream.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructor
libavcodec_DecoderContext::libavcodec_DecoderContext(const Decoder &decoder, Stream &stream, AVCodec *codec, const BijectiveMap<NamedPixelFormat, AVPixelFormat> &libavPixelFormatMap)
	: DecoderContext(decoder), libavPixelFormatMap(libavPixelFormatMap), stream(stream)
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
			break;
		ASSERT(ret == 0, u8"TODO: implement error handling");
		/*else if (ret < 0)
			return; //an error occured. skip packet*/
					
		//ready frame
		switch (this->codecContext->codec_type)
		{
		case AVMEDIA_TYPE_AUDIO:
			this->MapAudioFrame();
			break;
		case AVMEDIA_TYPE_VIDEO:
			this->MapVideoFrame();
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	}

	//update stream if possible
	if ((this->stream.bitRate == 0) && (this->codecContext->bit_rate != 0))
		this->stream.bitRate = this->codecContext->bit_rate;
	
	switch (this->codecContext->codec_type)
	{
	case AVMEDIA_TYPE_AUDIO:
	{
		AudioStream &audioStream = (AudioStream &)this->stream;
		if ((audioStream.sampleRate == 0) && (this->codecContext->sample_rate != 0))
			audioStream.sampleRate = this->codecContext->sample_rate;
		if ((audioStream.nChannels == 0) && (this->codecContext->channels != 0))
			audioStream.nChannels = this->codecContext->channels;
	}
	break;
	}
}

//Private methods
void libavcodec_DecoderContext::MapAudioFrame()
{
	AudioFrame *audioFrame;
	switch (this->frame->format)
	{
	case AV_SAMPLE_FMT_FLTP:
	{
		AudioBuffer<float32> *buffer = new AudioBuffer<float32>(this->MapChannels(this->frame->channels), (uint32)this->frame->nb_samples);
		for (uint32 i = 0; i < this->frame->channels; i++)
			MemCopy(buffer->GetChannel((Channel)i), this->frame->data[i], this->frame->nb_samples * sizeof(float32));
		audioFrame = new AudioFrame(buffer);
	}
	break;
	default:
		NOT_IMPLEMENTED_ERROR;
	}

	audioFrame->pts = this->frame->pts;

	this->AddFrame(audioFrame);
}

ChannelLayout libavcodec_DecoderContext::MapChannels(int nChannels)
{
	switch (nChannels)
	{
	case 1:
		return ChannelLayout::Mono;
	case 2:
		return ChannelLayout::Stereo;
	}

	NOT_IMPLEMENTED_ERROR;
}

void libavcodec_DecoderContext::MapPacket(const StdXX::Multimedia::Packet &packet)
{
	this->packet->data = (uint8_t *)packet.GetData();
	this->packet->size = packet.GetSize();
}

void libavcodec_DecoderContext::MapVideoFrame()
{
	Pixmap *pixmap = new Pixmap(Math::Size<uint16>(this->frame->width, this->frame->height), this->libavPixelFormatMap.GetReverse((AVPixelFormat)this->frame->format));
	for (uint8 i = 0; i < pixmap->GetPixelFormat().nPlanes; i++)
	{
		for (uint32 line = 0; line < pixmap->GetNumberOfLines(i); line++)
		{
			uint32 libav_lineIndex = (pixmap->GetNumberOfLines(i) - line - 1) * this->frame->linesize[i];
			void *dst = ((byte *)pixmap->GetPlane(i)) + line * pixmap->GetLineSize(i);
			MemCopy(dst, &this->frame->data[i][libav_lineIndex], Math::Min((uint32)this->frame->linesize[i], pixmap->GetLineSize(i))); //min size because of alignment
		}
	}

	VideoFrame *frame = new VideoFrame(pixmap);
	frame->pts = this->frame->pts;

	this->AddFrame(frame);
}
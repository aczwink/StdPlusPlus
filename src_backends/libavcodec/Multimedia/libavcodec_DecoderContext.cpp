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
	: DecoderContext(decoder, stream),
	libavPixelFormatMap(libavPixelFormatMap), extradata(nullptr)
{
	this->codecContext = avcodec_alloc_context3(codec);
	this->packet = av_packet_alloc();
	this->frame = av_frame_alloc();

	//set what some codecs don't have in its encoded data
	switch (stream.GetType())
	{
	case DataType::Video:
	{
		VideoStream &videoStream = (VideoStream &)stream;

		if (videoStream.size.width)
			this->codecContext->width = videoStream.size.width;
		if (videoStream.size.height)
			this->codecContext->height = videoStream.size.height;
	}
	break;
	}
	
	if (stream.codecPrivateData.HasValue())
	{
		this->codecContext->extradata_size = stream.codecPrivateData->GetNumberOfElements();
		this->extradata = (byte*)MemAlloc(this->codecContext->extradata_size + AV_INPUT_BUFFER_PADDING_SIZE);
		MemCopy(this->extradata, &(*stream.codecPrivateData)[0], this->codecContext->extradata_size);
		this->codecContext->extradata = this->extradata;
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
	MemFree(this->extradata);
}

//Public methods
void libavcodec_DecoderContext::Decode(const Packet & packet)
{
	this->MapPacket(packet);

	int ret = avcodec_send_packet(this->codecContext, this->packet);
	switch (ret)
	{
	case 0:
		//ok
		break;
	case AVERROR_INVALIDDATA:
		return; //well nothing we can do... data is corrupt
	default:
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
	
	while (ret >= 0)
	{
		ret = avcodec_receive_frame(this->codecContext, this->frame);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			break;
		ASSERT(ret == 0, u8"TODO: implement error handling");
		/*else if (ret < 0)
			return; //an error occured. skip packet*/
			
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
			if ((!audioStream.sampleFormat.HasValue()) && (this->codecContext->sample_fmt != AV_SAMPLE_FMT_NONE))
			{
				audioStream.sampleFormat = AudioSampleFormat(this->codecContext->channels, this->MapSampleFormat(this->codecContext->sample_fmt), av_sample_fmt_is_planar(this->codecContext->sample_fmt) == 1);
			}
		}
		break;
		case AVMEDIA_TYPE_VIDEO:
		{
			VideoStream &videoStream = (VideoStream &)this->stream;
			if (!videoStream.pixelFormat.HasValue() && (this->frame->format != -1))
			{
				NamedPixelFormat name = this->libavPixelFormatMap.GetReverse((AVPixelFormat)this->frame->format);
				videoStream.pixelFormat = PixelFormat(name);
			}
		}
		break;
		}
					
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
}

//Private methods
uint64 libavcodec_DecoderContext::GetBestFramePTS() const
{
	int64_t pts = this->frame->pts;
	if (pts == AV_NOPTS_VALUE)
		pts = av_frame_get_best_effort_timestamp(this->frame);

	if (pts == AV_NOPTS_VALUE)
		return Natural<uint64>::Max();
	return pts;
}

void libavcodec_DecoderContext::MapAudioFrame()
{
	AudioStream &audioStream = (AudioStream &)this->stream;
	AudioBuffer *audioBuffer = new AudioBuffer(this->frame->nb_samples, *audioStream.sampleFormat);
	for (uint8 i = 0; i < audioStream.sampleFormat->nPlanes; i++)
	{
		auto libav_lineSize = this->frame->linesize[i];
		if (libav_lineSize == 0) //The official doc tells us "For audio, only linesize[0] may be set. For planar audio, each channel plane must be the same size.". How is this ugly....
			libav_lineSize = this->frame->linesize[0];

		MemCopy(audioBuffer->GetPlane(i), this->frame->data[i], Math::Min((uint32)libav_lineSize, audioBuffer->GetPlaneSize(i))); //min size because of alignment
	}

	AudioFrame *audioFrame = new AudioFrame(audioBuffer);
	audioFrame->pts = this->GetBestFramePTS();

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

	if (packet.pts == Natural<uint64>::Max())
		this->packet->pts = AV_NOPTS_VALUE;
	else
		this->packet->pts = packet.pts;
}

AudioSampleType libavcodec_DecoderContext::MapSampleFormat(AVSampleFormat sampleFormat) const
{
	switch (sampleFormat)
	{
	case AV_SAMPLE_FMT_FLTP:
		return AudioSampleType::Float;
	}

	NOT_IMPLEMENTED_ERROR;
	return AudioSampleType::S16;
}

void libavcodec_DecoderContext::MapVideoFrame()
{
	VideoStream &videoStream = (VideoStream &)this->stream;

	Pixmap *pixmap = new Pixmap(Math::Size<uint16>(this->frame->width, this->frame->height), videoStream.pixelFormat.Value());
	for (uint8 i = 0; i < pixmap->GetPixelFormat().nPlanes; i++)
	{
		for (uint32 line = 0; line < pixmap->GetNumberOfLines(i); line++)
		{
			//uint32 libav_lineIndex = (pixmap->GetNumberOfLines(i) - line - 1) * this->frame->linesize[i];
			uint32 libav_lineIndex = (line) * this->frame->linesize[i];
			void *dst = ((byte *)pixmap->GetPlane(i)) + line * pixmap->GetLineSize(i);
			MemCopy(dst, &this->frame->data[i][libav_lineIndex], Math::Min((uint32)this->frame->linesize[i], pixmap->GetLineSize(i))); //min size because of alignment
		}
	}

	VideoFrame *frame = new VideoFrame(pixmap);
	frame->pts = this->GetBestFramePTS();

	this->AddFrame(frame);
}
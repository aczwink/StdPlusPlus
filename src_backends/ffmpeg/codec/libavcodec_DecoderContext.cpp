/*
* Copyright (c) 2018-2022 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/_Backends/ExtensionManager.hpp>
#include "../libavcodec_Extension.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructor
libavcodec_DecoderContext::libavcodec_DecoderContext(const Decoder &decoder, Stream &stream, const AVCodec *codec)
	: DecoderContext(decoder, stream), extradata(nullptr)
{
	this->codecContext = avcodec_alloc_context3(codec);
	this->frame = av_frame_alloc();

	//set what some codecs don't have in its encoded data
	switch (stream.codingParameters.dataType)
	{
		case DataType::Audio:
		{
			AudioStream& audioStream = (AudioStream&)stream;
			this->codecContext->channels = audioStream.sampleFormat->nChannels;
		}
		break;
	case DataType::Video:
	{
		VideoStream &videoStream = (VideoStream &)stream;

		if (videoStream.codingParameters.video.size.width)
			this->codecContext->width = videoStream.codingParameters.video.size.width;
		if (videoStream.codingParameters.video.size.height)
			this->codecContext->height = videoStream.codingParameters.video.size.height;
	}
	break;
	}
	
	if (stream.codingParameters.codecPrivateData.HasValue())
	{
		const libavcodec_Extension *libavcodecExtension = ExtensionManager::GetRootInstance().GetExtension<libavcodec_Extension>();

		this->extradata = libavcodecExtension->MapCodecExtradata(*stream.codingParameters.codecPrivateData);
		this->codecContext->extradata_size = stream.codingParameters.codecPrivateData->Size();
		this->codecContext->extradata = this->extradata;
	}

	int ret = avcodec_open2(this->codecContext, codec, nullptr);
	ASSERT(ret == 0, u8"TODO: not implemented on failure...");
}

//Destructor
libavcodec_DecoderContext::~libavcodec_DecoderContext()
{
	MemFree(this->extradata);
	this->codecContext->extradata = nullptr;
	this->codecContext->extradata_size = 0;

	av_frame_free(&this->frame);
	avcodec_free_context(&this->codecContext);
}

//Public methods
void libavcodec_DecoderContext::Decode(const IPacket & packet)
{
	const libavcodec_Extension *libavcodecExtension = ExtensionManager::GetRootInstance().GetExtension<libavcodec_Extension>();

	AVPacket avPacket;
	libavcodecExtension->MapPacket(packet, avPacket);

	int ret = avcodec_send_packet(this->codecContext, &avPacket);
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
		if ((this->stream.codingParameters.bitRate == 0) && (this->codecContext->bit_rate != 0))
			this->stream.codingParameters.bitRate = this->codecContext->bit_rate;

		const libavcodec_Extension *libavcodecExtension = ExtensionManager::GetRootInstance().GetExtension<libavcodec_Extension>();
		switch (this->codecContext->codec_type)
		{
		case AVMEDIA_TYPE_AUDIO:
		{
			AudioStream &audioStream = (AudioStream &)this->stream;
			if ((audioStream.codingParameters.audio.sampleRate == 0) && (this->codecContext->sample_rate != 0))
				audioStream.codingParameters.audio.sampleRate = this->codecContext->sample_rate;
			if ((!audioStream.sampleFormat.HasValue()) && (this->codecContext->sample_fmt != AV_SAMPLE_FMT_NONE))
			{
				audioStream.sampleFormat = libavcodecExtension->MapAudioSampleFormat(this->codecContext->channels, this->codecContext->channel_layout, this->codecContext->sample_fmt);
			}
		}
		break;
		case AVMEDIA_TYPE_VIDEO:
		{
			VideoStream &videoStream = (VideoStream &)this->stream;
			if (!videoStream.pixelFormat.HasValue() && (this->frame->format != -1))
			{
				videoStream.pixelFormat = libavcodecExtension->MapPixelFormat(static_cast<AVPixelFormat>(this->frame->format));
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

void libavcodec_DecoderContext::Reset()
{
	avcodec_flush_buffers(this->codecContext);

	DecoderContext::Reset();
}

//Private methods
uint64 libavcodec_DecoderContext::GetBestFramePTS() const
{
	int64_t pts = this->frame->pts;
	if (pts == AV_NOPTS_VALUE)
		pts = this->frame->best_effort_timestamp;

	if (pts == AV_NOPTS_VALUE)
		return Unsigned<uint64>::Max();
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
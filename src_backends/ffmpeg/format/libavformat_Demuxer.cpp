/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include "libavformat_Demuxer.hpp"
//Local
#include <Std++/Multimedia/AudioStream.hpp>
#include <Std++/Multimedia/VideoStream.hpp>
#include <Std++/Multimedia/SubtitleStream.hpp>
#include <Std++/_Backends/ExtensionManager.hpp>
#include "libavformat_Format.hpp"
#include "../libav_Packet.hpp"
#include "../libavformat_Extension.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

libavformat_Demuxer::libavformat_Demuxer(const libavformat_Format& format, SeekableInputStream &seekableInputStream)
	: Demuxer(format, seekableInputStream), streamWrapper(seekableInputStream)
{
	this->avInputFormat = format.InputFormat();
	this->fmt_ctx = avformat_alloc_context();
	this->fmt_ctx->pb = this->streamWrapper.Context();
}

//Destructor
libavformat_Demuxer::~libavformat_Demuxer()
{
	avformat_close_input(&this->fmt_ctx);
}

//Public methods
void libavformat_Demuxer::ReadHeader()
{
	int ret = avformat_open_input(&this->fmt_ctx, nullptr, this->avInputFormat, nullptr);
	ASSERT_EQUALS(0, ret);

	ret = avformat_find_stream_info(this->fmt_ctx, nullptr);
	ASSERT_EQUALS(0, ret);

	const libavformat_Extension *libavformatExtension = ExtensionManager::GetRootInstance().GetExtension<libavformat_Extension>();
	for(unsigned int i = 0; i < this->fmt_ctx->nb_streams; i++)
	{
		AVStream* avStream = this->fmt_ctx->streams[i];

		Stream *stream = nullptr;
		switch(avStream->codecpar->codec_type)
		{
			case AVMEDIA_TYPE_AUDIO:
				AudioStream* audioStream;
				stream = audioStream = new AudioStream;
				audioStream->sampleRate = avStream->codecpar->sample_rate;
				audioStream->sampleFormat = libavformatExtension->MapAudioSampleFormat(avStream->codecpar->channels, (AVSampleFormat)avStream->codecpar->format);
				break;
			case AVMEDIA_TYPE_VIDEO:
				VideoStream* videoStream;
				stream = videoStream = new VideoStream;
				videoStream->pixelFormat = libavformatExtension->MapPixelFormat(static_cast<AVPixelFormat>(avStream->codecpar->format));
				videoStream->size = Math::Size<uint16>(avStream->codecpar->width, avStream->codecpar->height);
				break;
			case AVMEDIA_TYPE_SUBTITLE:
				stream = new SubtitleStream;
				break;
		}
		stream->startTime = avStream->start_time;
		stream->duration = avStream->duration;
		stream->timeScale = {static_cast<uint64>(avStream->time_base.num), static_cast<uint64>(avStream->time_base.den)};
		stream->bitRate = avStream->codecpar->bit_rate;
		stream->SetCodingFormat(libavformatExtension->MapCodecId(avStream->codecpar->codec_id));

		this->AddStream(stream);
	}
}

//Private methods
UniquePointer<IPacket> libavformat_Demuxer::ReadPacket()
{
	UniquePointer<libav_Packet> packet = new libav_Packet;
	int ret = av_read_frame(this->fmt_ctx, &packet->pkt);
	if(ret == AVERROR_EOF)
		return nullptr;
	ASSERT_EQUALS(0, ret);

	return packet;
}

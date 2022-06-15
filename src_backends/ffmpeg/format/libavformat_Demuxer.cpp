/*
 * Copyright (c) 2020,2022 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/Stream.hpp>
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
	this->fmt_ctx->pb = nullptr;
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
		AVCodecParameters *codecParameters = avStream->codecpar;
		switch(codecParameters->codec_type)
		{
			case AVMEDIA_TYPE_AUDIO:
				AudioStream* audioStream;
				stream = audioStream = new AudioStream;
				audioStream->codingParameters.audio.sampleRate = codecParameters->sample_rate;
				audioStream->sampleFormat = libavformatExtension->MapAudioSampleFormat(codecParameters->channels, codecParameters->channel_layout, (AVSampleFormat) codecParameters->format);
				break;
			case AVMEDIA_TYPE_VIDEO:
				VideoStream* videoStream;
				stream = videoStream = new VideoStream;
				videoStream->pixelFormat = libavformatExtension->MapPixelFormat(static_cast<AVPixelFormat>(codecParameters->format));
				videoStream->codingParameters.video.size = Math::Size<uint16>(codecParameters->width, codecParameters->height);
				break;
			case AVMEDIA_TYPE_SUBTITLE:
				stream = new SubtitleStream;
				break;
		}
		stream->codingParameters.bitRate = codecParameters->bit_rate;
		stream->SetCodingFormat(libavformatExtension->MapCodecId(codecParameters->codec_id));

		if(codecParameters->extradata_size > 0)
			stream->codingParameters.codecPrivateData = FixedSizeBuffer(codecParameters->extradata, codecParameters->extradata_size);

		stream->startTime = avStream->start_time;
		stream->duration = avStream->duration;
		stream->timeScale = {static_cast<uint64>(avStream->time_base.num), static_cast<uint64>(avStream->time_base.den)};

		NOT_IMPLEMENTED_ERROR; //TODO: reimplement next line
		/*
		switch(avStream->need_parsing)
		{
			case AVSTREAM_PARSE_NONE:
				stream->parserFlags.requiresParsing = false;
				stream->parserFlags.repack = false;
				break;
			case AVSTREAM_PARSE_FULL:
				stream->parserFlags.requiresParsing = true;
				stream->parserFlags.repack = true;
				break;
			default:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}*/

		this->AddStream(stream);
	}
}

void libavformat_Demuxer::Seek(uint64 timestamp, const class TimeScale &timeScale)
{
	class TimeScale avTimeScale(AV_TIME_BASE_Q.num, AV_TIME_BASE_Q.den);
	uint64 convertedTs = timeScale.Rescale(timestamp, avTimeScale);
	int ret = avformat_seek_file(this->fmt_ctx, -1, convertedTs, convertedTs, convertedTs, 0);
	ASSERT(ret >= 0, u8"TODO: implement this correctly");
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
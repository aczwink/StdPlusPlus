/*
 * Copyright (c) 2020-2024 Amir Czwink (amir130@hotmail.de)
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
#include "libavformat_Muxer.hpp"
//Local
#include <Std++/_Backends/ExtensionManager.hpp>
#include "libavformat_Format.hpp"
#include "../libavformat_Extension.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructor
libavformat_Muxer::libavformat_Muxer(const libavformat_Format &format, SeekableOutputStream &seekableOutputStream)
	: Muxer(format, seekableOutputStream)
{
	this->avOutputFormat = format.OutputFormat();

	int ret = avformat_alloc_output_context2(&this->fmt_ctx, format.OutputFormat(), nullptr, nullptr);
	ASSERT(ret >= 0, u8"TODO: DO THIS CORRECTLY");

	if (!(this->avOutputFormat->flags & AVFMT_NOFILE))
	{
		this->streamWrapper = new libavio_OutputStreamWrapper(seekableOutputStream);
		this->fmt_ctx->pb = this->streamWrapper->Context();
	}
}

//Destructor
libavformat_Muxer::~libavformat_Muxer()
{
	for(uint8* extradata : this->extradata)
		MemFree(extradata);
	for(uint32 i = 0; i < this->fmt_ctx->nb_streams; i++)
		this->fmt_ctx->streams[i]->codecpar->extradata = nullptr;

	this->streamWrapper = nullptr;
	this->fmt_ctx->pb = nullptr;
	avformat_free_context(this->fmt_ctx);
}

//Public methods
void libavformat_Muxer::Finalize()
{
	int ret = av_write_trailer(this->fmt_ctx);
	ASSERT_EQUALS(0, ret);
}

void libavformat_Muxer::WriteHeader()
{
	const libavformat_Extension *libavformatExtension = ExtensionManager::GetRootInstance().GetExtension<libavformat_Extension>();
	for(uint32 i = 0; i < this->GetNumberOfStreams(); i++)
	{
		Stream* stream = this->GetStream(i);
		const DecodingParameters& cp = stream->codingParameters;

		AVStream* out_stream = avformat_new_stream(this->fmt_ctx, nullptr);
		AVCodecParameters *codecpar = out_stream->codecpar;

		switch(stream->codingParameters.dataType)
		{
			case DataType::Audio:
				codecpar->codec_type = AVMEDIA_TYPE_AUDIO;
				codecpar->sample_rate = stream->codingParameters.audio.sampleRate;
				break;
			case DataType::Subtitle:
				codecpar->codec_type = AVMEDIA_TYPE_SUBTITLE;
				break;
			case DataType::Video:
				codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
				codecpar->width = stream->codingParameters.video.size.width;
				codecpar->height = stream->codingParameters.video.size.height;
				break;
		}

		codecpar->codec_id = libavformatExtension->MapCodingFormat(stream->codingParameters.codingFormat);
		codecpar->bit_rate = codecpar->bit_rate;
		if(cp.codecPrivateData.HasValue())
		{
			codecpar->extradata = libavformatExtension->MapCodecExtradata(*cp.codecPrivateData);
			this->extradata.Push(codecpar->extradata);
			codecpar->extradata_size = cp.codecPrivateData->Size();
		}
	}

	int ret = avformat_write_header(this->fmt_ctx, nullptr);
	ASSERT(ret >= 0, u8"TODO: DO THIS CORRECTLY");
	
	//avformat_write_header changes the time_base
	for(uint32 i = 0; i < this->GetNumberOfStreams(); i++)
	{
		Stream* stream = this->GetStream(i);

		const AVRational &timeBase = this->fmt_ctx->streams[i]->time_base;
		stream->timeScale = {static_cast<uint64>(timeBase.num), static_cast<uint64>(timeBase.den) };
	}
}

void libavformat_Muxer::WritePacket(const IPacket& packet)
{
	AVPacket pkt;

	const libavformat_Extension *libavformatExtension = ExtensionManager::GetRootInstance().GetExtension<libavformat_Extension>();
	libavformatExtension->MapPacket(packet, pkt);

	int ret = av_interleaved_write_frame(this->fmt_ctx, &pkt);
	ASSERT(ret >= 0, u8"TODO: DO THIS CORRECTLY");
}
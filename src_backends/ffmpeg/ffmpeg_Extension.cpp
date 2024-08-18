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
#include "ffmpeg_Extension.hpp"
//Local
#include <Std++/Debug.hpp>
#include <Std++/Multimedia/AudioSampleFormat.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Public methods
void ffmpeg_Extension::Load()
{
	Extension::Load();

	this->LoadPixelFormatMap();
}

AudioSampleFormat ffmpeg_Extension::MapAudioSampleFormat(const AVChannelLayout& channelLayout, AVSampleFormat sampleFormat) const
{
	AudioSampleType sampleType;
	switch (sampleFormat)
	{
		case AV_SAMPLE_FMT_FLTP:
			sampleType = AudioSampleType::Float;
			break;
		default:
			NOT_IMPLEMENTED_ERROR;
	}

	AudioSampleFormat audioSampleFormat = AudioSampleFormat(channelLayout.nb_channels, sampleType, av_sample_fmt_is_planar(sampleFormat) == 1);

	switch(channelLayout.order)
	{
		case AV_CHANNEL_ORDER_NATIVE:
		{
			switch(channelLayout.nb_channels)
			{
				case 1:
				case 2:
					break; //don't need to do something here
				case AV_CH_LAYOUT_5POINT1:
					NOT_IMPLEMENTED_ERROR; //TODO: reimplement me
					audioSampleFormat.channels[0].speaker = SpeakerPosition::Front_Left;
					audioSampleFormat.channels[1].speaker = SpeakerPosition::Front_Right;
					audioSampleFormat.channels[2].speaker = SpeakerPosition::Front_Center;
					audioSampleFormat.channels[3].speaker = SpeakerPosition::LowFrequency;
					audioSampleFormat.channels[4].speaker = SpeakerPosition::Side_Left;
					audioSampleFormat.channels[5].speaker = SpeakerPosition::Side_Right;
					break;
				default:
					NOT_IMPLEMENTED_ERROR;
			}
		}
		break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	return audioSampleFormat;
}

uint8 *ffmpeg_Extension::MapCodecExtradata(const FixedSizeBuffer& codecPrivateData) const
{
	uint8* ptr = (uint8*)MemAlloc(codecPrivateData.Size() + AV_INPUT_BUFFER_PADDING_SIZE);
	MemCopy(ptr, codecPrivateData.Data(), codecPrivateData.Size());
	MemZero(ptr + codecPrivateData.Size(), AV_INPUT_BUFFER_PADDING_SIZE);

	return ptr;
}

void ffmpeg_Extension::MapPacket(const IPacket& sourcePacket, AVPacket& destPacket) const
{
	destPacket.buf = nullptr;

	if (sourcePacket.GetPresentationTimestamp() == Unsigned<uint64>::Max())
		destPacket.pts = AV_NOPTS_VALUE;
	else
		destPacket.pts = sourcePacket.GetPresentationTimestamp();

	if (sourcePacket.GetDecodeTimestamp() == Unsigned<uint64>::Max())
		destPacket.dts = AV_NOPTS_VALUE;
	else
		destPacket.dts = sourcePacket.GetDecodeTimestamp();

	destPacket.data = const_cast<uint8*>(sourcePacket.GetData());
	destPacket.size = sourcePacket.GetSize();
	destPacket.stream_index = sourcePacket.GetStreamIndex();
	destPacket.flags = sourcePacket.ContainsKeyFrame() ? AV_PKT_FLAG_KEY : 0;
	destPacket.side_data = nullptr;
	destPacket.side_data_elems = 0;
	destPacket.duration = 0;
	destPacket.pos = -1;
}

void ffmpeg_Extension::Unload()
{
}

//Protected methods
void ffmpeg_Extension::LoadCodingFormatIdMap()
{
	//audio
	this->libavCodecIdMap.Insert(AV_CODEC_ID_AC3, CodingFormatId::AC3); //ac3 is now patent-free :)
	this->libavCodecIdMap.Insert(AV_CODEC_ID_MP3, CodingFormatId::MP3); //mp3 is now patent-free :)
	this->libavCodecIdMap.Insert(AV_CODEC_ID_PCM_S16BE, CodingFormatId::PCM_S16BE);
	this->libavCodecIdMap.Insert(AV_CODEC_ID_PCM_S16LE, CodingFormatId::PCM_S16LE);
	this->libavCodecIdMap.Insert(AV_CODEC_ID_PCM_S8, CodingFormatId::PCM_S8);
	this->libavCodecIdMap.Insert(AV_CODEC_ID_PCM_U8, CodingFormatId::PCM_U8);
	this->libavCodecIdMap.Insert(AV_CODEC_ID_VORBIS, CodingFormatId::Vorbis);

	//video
	this->libavCodecIdMap.Insert(AV_CODEC_ID_PNG, CodingFormatId::PNG);
	this->libavCodecIdMap.Insert(AV_CODEC_ID_RAWVIDEO, CodingFormatId::RawVideo);
	this->libavCodecIdMap.Insert(AV_CODEC_ID_THEORA, CodingFormatId::Theora);
}

void ffmpeg_Extension::LoadPatentedCodingFormatIdMap()
{
	//audio
	this->libavCodecIdMap.Insert(AV_CODEC_ID_AAC, CodingFormatId::AAC);

	//video
	this->libavCodecIdMap.Insert(AV_CODEC_ID_H264, CodingFormatId::H264);
	this->libavCodecIdMap.Insert(AV_CODEC_ID_MPEG4, CodingFormatId::MPEG4Part2);
	this->libavCodecIdMap.Insert(AV_CODEC_ID_MSMPEG4V2, CodingFormatId::MS_MPEG4Part2V2);
}

//Private methods
void ffmpeg_Extension::LoadPixelFormatMap()
{
	this->libavPixelFormatMap.Insert(NamedPixelFormat::BGR_24, AV_PIX_FMT_BGR24);
	this->libavPixelFormatMap.Insert(NamedPixelFormat::RGB_24, AV_PIX_FMT_RGB24);
	this->libavPixelFormatMap.Insert(NamedPixelFormat::YCbCr_420_P, AV_PIX_FMT_YUV420P);
}
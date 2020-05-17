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
#include "ffmpeg_Extension.hpp"
//Local
#include <Std++/Debug.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX::Multimedia;

//Public methods
void ffmpeg_Extension::Load()
{
	Extension::Load();

	this->LoadPixelFormatMap();
}

AudioSampleFormat ffmpeg_Extension::MapAudioSampleFormat(int nChannels, AVSampleFormat sampleFormat) const
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
	return AudioSampleFormat(nChannels, sampleType, av_sample_fmt_is_planar(sampleFormat) == 1);
}

//Protected methods
void ffmpeg_Extension::LoadCodingFormatIdMap()
{
	//audio
	this->libavCodecIdMap.Insert(AV_CODEC_ID_MP3, CodingFormatId::MP3); //mp3 is now patent-free :)
	this->libavCodecIdMap.Insert(AV_CODEC_ID_PCM_S16LE, CodingFormatId::PCM_S16LE);
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

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
#include "libavcodec_Backend.hpp"
//Local
#include <Std++/Containers/Map/Map.hpp>
#include "Multimedia/libavcodec_Decoder.hpp"
#include "Multimedia/libavcodec_Encoder.hpp"
#include "Multimedia/libavcodec_Parser.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Public methods
void libavcodec_Backend::Load()
{
	avcodec_register_all();

	this->LoadPixelFormatMap();
	this->LoadCodingFormatIdMap();
#ifdef _STDXX_EXTENSION_LIBAVCODEC_ENABLE_PATENDED
	this->LoadPatentedCodingFormatIdMap();
#endif

	//register
	for (const auto &kv : this->libavCodecIdMap)
	{
		this->RegisterDecoderIfAvailable(kv.value, kv.key);
		this->RegisterEncoderIfAvailable(kv.value, kv.key);
		this->RegisterParserIfAvailable(kv.value, kv.key);
	}


	/*
	//video
	g_libavcodec_codec_map.Insert(CodecId::H264, AV_CODEC_ID_H264);
	*/
}

//Private methods
void libavcodec_Backend::LoadCodingFormatIdMap()
{
	this->libavCodecIdMap.Insert(AV_CODEC_ID_MP3, CodingFormatId::MP3); //mp3 is now patent-free :)
	this->libavCodecIdMap.Insert(AV_CODEC_ID_PCM_S16LE, CodingFormatId::PCM_S16LE);
	this->libavCodecIdMap.Insert(AV_CODEC_ID_PNG, CodingFormatId::PNG);
}

#ifdef _STDXX_EXTENSION_LIBAVCODEC_ENABLE_PATENDED
void libavcodec_Backend::LoadPatentedCodingFormatIdMap()
{
	this->libavCodecIdMap.Insert(AV_CODEC_ID_MSMPEG4V2, CodingFormatId::MS_MPEG4Part2V2);
}
#endif

void libavcodec_Backend::LoadPixelFormatMap()
{
	this->libavPixelFormatMap.Insert(NamedPixelFormat::RGB_24, AV_PIX_FMT_RGB24);
	this->libavPixelFormatMap.Insert(NamedPixelFormat::YCbCr_420_P, AV_PIX_FMT_YUV420P);
}

void libavcodec_Backend::RegisterDecoderIfAvailable(CodingFormatId codingFormatId, AVCodecID libavCodecId)
{
	AVCodec *codec = avcodec_find_decoder(libavCodecId);
	if (codec)
		Decoder::Register(new libavcodec_Decoder(this->libavPixelFormatMap, codingFormatId, libavCodecId), 0.5f);
}

void libavcodec_Backend::RegisterEncoderIfAvailable(CodingFormatId codingFormatId, AVCodecID libavCodecId)
{
	AVCodec *codec = avcodec_find_encoder(libavCodecId);
	if(codec)
		Encoder::Register(new libavcodec_Encoder(this->libavPixelFormatMap, codingFormatId, libavCodecId), 0.5f);
}

void libavcodec_Backend::RegisterParserIfAvailable(CodingFormatId codingFormatId, AVCodecID libavCodecId)
{
	AVCodecParserContext *parserContext = av_parser_init(libavCodecId);
	if (parserContext)
	{
		Parser::Register(new libavcodec_Parser(libavCodecIdMap, codingFormatId, libavCodecId), 0.5f);
		av_parser_close(parserContext);
	}
}
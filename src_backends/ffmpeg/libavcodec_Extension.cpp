/*
 * Copyright (c) 2018-2021 Amir Czwink (amir130@hotmail.de)
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
#include "libavcodec_Extension.hpp"
//Local
#include <Std++/Containers/BinaryTreeMap/BinaryTreeMap.hpp>
#include "codec/libavcodec_Decoder.hpp"
#include "codec/libavcodec_Encoder.hpp"
#include "codec/libavcodec_Parser.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Public methods
void libavcodec_Extension::Load()
{
	ffmpeg_Extension::Load();

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
}

//Private methods
void libavcodec_Extension::RegisterDecoderIfAvailable(CodingFormatId codingFormatId, AVCodecID libavCodecId)
{
	AVCodec *codec = avcodec_find_decoder(libavCodecId);
	if (codec)
		Decoder::Register(new libavcodec_Decoder(codingFormatId, libavCodecId), 0.5f);
}

void libavcodec_Extension::RegisterEncoderIfAvailable(CodingFormatId codingFormatId, AVCodecID libavCodecId)
{
	AVCodec *codec = avcodec_find_encoder(libavCodecId);
	if(codec)
		Encoder::Register(new libavcodec_Encoder(codingFormatId, libavCodecId), 0.5f);
}

void libavcodec_Extension::RegisterParserIfAvailable(CodingFormatId codingFormatId, AVCodecID libavCodecId)
{
	AVCodecParserContext *parserContext = av_parser_init(libavCodecId);
	if (parserContext)
	{
		Parser::Register(new libavcodec_Parser(libavCodecIdMap, codingFormatId, libavCodecId), 0.5f);
		av_parser_close(parserContext);
	}
}
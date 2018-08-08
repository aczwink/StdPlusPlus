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
#include "Multimedia/libavcodec_Parser.hpp"
#include "../../../src/Multimedia/CodingFormatIdMap.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Local functions
static void RegisterParserIfAvailable(CodingFormatId codingFormatId, AVCodecID libavCodecId)
{
	AVCodecParserContext *parserContext = av_parser_init(libavCodecId);
	if (parserContext)
	{
		Parser::Register(new libavcodec_Parser(codingFormatId, libavCodecId), 0.5f);
		av_parser_close(parserContext);
	}
}

//Public methods
void libavcodec_Backend::Load()
{
	avcodec_register_all();

	CodingFormatIdMap<AVCodecID> libavCodecIdMap;

	//patented
#ifdef _STDXX_EXTENSION_LIBAVCODEC_ENABLE_PATENDED
	libavCodecIdMap.Insert(AV_CODEC_ID_MSMPEG4V2, CodingFormatId::MS_MPEG4Part2V2);
#endif

	for (const auto &kv : libavCodecIdMap)
	{
		RegisterParserIfAvailable(kv.value, kv.key);
		Decoder::Register(new libavcodec_Decoder(kv.value, kv.key), 0.5f);
	}


	/*
	//audio
	g_libavcodec_codec_map.Insert(CodecId::MP3, AV_CODEC_ID_MP3);

	//video
	g_libavcodec_codec_map.Insert(CodecId::H264, AV_CODEC_ID_H264);
	g_libavcodec_codec_map.Insert(CodecId::PNG, AV_CODEC_ID_PNG);
	*/
}
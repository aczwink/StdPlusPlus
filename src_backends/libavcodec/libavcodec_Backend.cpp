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
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

//Global variables
StdXX::Map<StdXX::Multimedia::CodecId, AVCodecID> g_libavcodec_codec_map;

//Public methods
void libavcodec_Backend::Load()
{
	avcodec_register_all();

	//audio
	g_libavcodec_codec_map.Insert(CodecId::MP3, AV_CODEC_ID_MP3);

	//video
	g_libavcodec_codec_map.Insert(CodecId::H264, AV_CODEC_ID_H264);
	g_libavcodec_codec_map.Insert(CodecId::MS_MPEG4Part2V2, AV_CODEC_ID_MSMPEG4V2);
	g_libavcodec_codec_map.Insert(CodecId::PNG, AV_CODEC_ID_PNG);
}

void libavcodec_Backend::Unload() const
{
	g_libavcodec_codec_map.Release();
}

//Namespace functions
AVCodecID StdXX::MapCodecId(CodecId codecId)
{
	auto it = g_libavcodec_codec_map.Find(codecId);
	if(it == g_libavcodec_codec_map.end())
		return AV_CODEC_ID_NONE;

	return (AVCodecID)(*it).value;
}
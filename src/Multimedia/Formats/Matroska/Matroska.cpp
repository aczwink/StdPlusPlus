/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
//Main header
#include "Matroska.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX::Multimedia;
/*
//Global variables
Map<String, CodecId> g_matroskaCodecStringMap;

//Local functions
static void LoadMap()
{
	static bool loaded = false;

	if(!loaded)
	{
		loaded = true;

		//Audio codecs
		g_matroskaCodecStringMap.Insert(u8"A_AAC", CodecId::AAC);
		g_matroskaCodecStringMap.Insert(u8"A_MPEG/L2", CodecId::MP2);

		//Video codecs
		g_matroskaCodecStringMap.Insert(u8"V_MPEG1", CodecId::MPEG1Video);
		g_matroskaCodecStringMap.Insert(u8"V_MPEG4/ISO/AVC", CodecId::H264);
	}
}

//Namespace functions
CodecId Matroska::MapCodecString(const String &codecString)
{
	LoadMap();

	if(g_matroskaCodecStringMap.Contains(codecString))
		return g_matroskaCodecStringMap[codecString];

	return CodecId::Unknown;
}

String Matroska::MapToCodecString(CodecId codecId)
{
	LoadMap();

	for(const auto &refKV : g_matroskaCodecStringMap)
		if(refKV.value == codecId)
			return refKV.key;

	return String();
}*/

CodingFormatIdMap<String> Matroska::GetCodingFormatMap()
{
	CodingFormatIdMap<String> matroskaCodecMap;

	//Audio coding formats
	matroskaCodecMap.Insert(u8"A_MPEG/L3", CodingFormatId::MP3);

	/*
	//audio codecs
	if (string == CODEC_PCM_FLOAT_LE)
	{
		this->parserState.currentTrack.codecPrivate.isFloatPCM = true;
		return CodecId::Unknown;
	}

	if (string == CODEC_PCM_INTEGER_LE)
	{
		this->parserState.currentTrack.codecPrivate.isIntegerPCM = true;
		return CodecId::Unknown;
	}
	*/

	return matroskaCodecMap;
}
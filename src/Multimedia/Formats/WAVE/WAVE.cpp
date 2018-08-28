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
#include "WAVE.h"
//Local
#include "../../CodingFormatIdMap.hpp"
//Namespaces
using namespace _stdxx_;
/*
//Functions
void AddMS_TwoCC_AudioCodecs(FiniteSet<CodecId> &refCodecSet)
{
	LoadMap();

	for(const auto &refKV : g_ms_audio_twoCC_map)
		refCodecSet.Insert(refKV.value);
}
*/
/*
//Local functions
static CodingFormatIdMap<uint16> LoadMap()
{
	CodingFormatIdMap<uint16> twoCCMap;
	
	twoCCMap.Insert(1, CodingFormatId::PCM_S16LE); //WAVE_FORMAT_PCM
	twoCCMap.Insert(3, CodingFormatId::PCM_Float32LE); //WAVE_FORMAT_IEEE_FLOAT
	twoCCMap.Insert(0x50, CodingFormatId::MP2); //TODO: could also be layer 1
}

uint16 MapToTwoCC(CodecId codecId)
{
	LoadMap();

	for (const auto &refKV : g_ms_audio_twoCC_map)
		if (refKV.value == codecId)
			return refKV.key;

	NOT_IMPLEMENTED_ERROR;
	return -1;
}*/
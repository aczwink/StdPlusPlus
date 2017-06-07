/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
//Local
#include <ACStdLib/Containers/Map/Map.hpp>
#include <ACStdLib/Containers/Strings/ByteString.hpp>
#include <ACStdLib/Multimedia/CodecId.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

/*
Sources:
https://www.matroska.org/technical/specs/index.html
https://www.matroska.org/technical/specs/codecid/index.html
http://haali.su/mkv/codecs.pdf
*/

namespace Matroska
{
	enum EMatroskaId
	{
		//EBML Header
		MATROSKA_ID_EBML = 0x1A45DFA3,
		MATROSKA_ID_EBMLVERSION = 0x4286,
		MATROSKA_ID_EBMLREADVERSION = 0x42F7,
		MATROSKA_ID_EBMLMAXIDLENGTH = 0x42F2,
		MATROSKA_ID_EBMLMAXSIZELENGTH = 0x42F3,
		MATROSKA_ID_DOCTYPE = 0x4282,
		MATROSKA_ID_DOCTYPEVERSION = 0x4287,
		MATROSKA_ID_DOCTYPEREADVERSION = 0x4285,
		//Segment
			MATROSKA_ID_SEGMENT = 0x18538067,
		//Meta Seek Information
			MATROSKA_ID_SEEKHEAD = 0x114D9B74,
		MATROSKA_ID_SEEK = 0x4DBB,
		MATROSKA_ID_SEEKID = 0x53AB,
		MATROSKA_ID_SEEKPOSITION = 0x53AC,
		//Segment Information
			MATROSKA_ID_INFO = 0x1549A966,
		MATROSKA_ID_TIMECODESCALE = 0x2AD7B1,
		MATROSKA_ID_DURATION = 0x4489,
		MATROSKA_ID_MUXINGAPP = 0x4D80,
		MATROSKA_ID_WRITINGAPP = 0x5741,
		//Cluster
			MATROSKA_ID_CLUSTER = 0x1F43B675,
		MATROSKA_ID_TIMECODE = 0xE7,
		MATROSKA_ID_BLOCK = 0xA1,
		MATROSKA_ID_SIMPLEBLOCK = 0xA3,
		//Track
			MATROSKA_ID_TRACKS = 0x1654AE6B,
		MATROSKA_ID_TRACKENTRY = 0xAE,
		MATROSKA_ID_TRACKNUMBER = 0xD7,
		MATROSKA_ID_TRACKUID = 0x73C5,
		MATROSKA_ID_TRACKTYPE = 0x83,
		MATROSKA_ID_CODECID = 0x86,
		MATROSKA_ID_CODECPRIVATE = 0x63A2,
		//Track -> Audio
			MATROSKA_ID_AUDIO = 0xE1,
		MATROSKA_ID_SAMPLINGFREQUENCY = 0xB5,
		MATROSKA_ID_CHANNELS = 0x9F,
		MATROSKA_ID_BITDEPTH = 0x6264,
		//Track -> Video
			MATROSKA_ID_VIDEO = 0xE0,
		MATROSKA_ID_PIXELWIDTH = 0xB0,
		MATROSKA_ID_PIXELHEIGHT = 0xBA,
		//Cueing Data
			MATROSKA_ID_CUES = 0x1C53BB6B,
		MATROSKA_ID_CUEPOINT = 0xBB,
		MATROSKA_ID_CUETIME = 0xB3,
		MATROSKA_ID_CUETRACKPOSITIONS = 0xB7,
		MATROSKA_ID_CUETRACK = 0xF7,
		MATROSKA_ID_CUECLUSTERPOSITION = 0xF1,
	};

	enum ETrackType
	{
		TRACK_TYPE_VIDEO = 1,
		TRACK_TYPE_AUDIO = 2,
		TRACK_TYPE_SUBTITLE = 0x11,
	};

	//Specific codecs
#define CODEC_PCM_FLOAT_LE "A_PCM/FLOAT/IEEE"
#define CODEC_PCM_INTEGER_LE "A_PCM/INT/LIT"
#define CODEC_MS_FOURCC "V_MS/VFW/FOURCC"

	//Functions
	CodecId MapCodecString(const ByteString &refCodecString);
	ByteString MapToCodecString(CodecId codecId);
}
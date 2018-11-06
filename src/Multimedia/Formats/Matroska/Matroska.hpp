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
#pragma once
//Local
#include <Std++/Containers/Map/Map.hpp>
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Containers/Array/DynamicArray.hpp>
#include <Std++/Multimedia/TimeIndex.hpp>
#include "../../CodingFormatIdMap.hpp"
#include "EBML.hpp"
//Namespaces
using namespace StdXX;

/*
Sources:
https://www.matroska.org/technical/specs/index.html
https://www.matroska.org/technical/specs/codecid/index.html
http://haali.su/mkv/codecs.pdf
*/

namespace Matroska
{
	enum MatroskaId
	{
		//EBML Header
		MATROSKA_ID_EBMLVERSION = 0x4286,
		MATROSKA_ID_EBMLREADVERSION = 0x42F7,
		MATROSKA_ID_EBMLMAXIDLENGTH = 0x42F2,
		MATROSKA_ID_EBMLMAXSIZELENGTH = 0x42F3,
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
		MATROSKA_ID_POSITION = 0xA7,
		MATROSKA_ID_PREVSIZE = 0xAB,
		MATROSKA_ID_BLOCKGROUP = 0xA0,
		MATROSKA_ID_BLOCK = 0xA1,
		MATROSKA_ID_SIMPLEBLOCK = 0xA3,
		MATROSKA_ID_BLOCKDURATION = 0x9B,
		MATROSKA_ID_REFERENCEBLOCK = 0xFB,
		//Track
			MATROSKA_ID_TRACKS = 0x1654AE6B,
		MATROSKA_ID_TRACKENTRY = 0xAE,
		MATROSKA_ID_TRACKNUMBER = 0xD7,
		MATROSKA_ID_TRACKUID = 0x73C5,
		MATROSKA_ID_TRACKTYPE = 0x83,
		MATROSKA_ID_DEFAULTDURATION = 0x23E383,
		MATROSKA_ID_CODECID = 0x86,
		MATROSKA_ID_CODECPRIVATE = 0x63A2,
		//Track -> Content encoding
		MATROSKA_ID_CONTENTENCODINGS = 0x6D80,
		MATROSKA_ID_CONTENTENCODING = 0x6240,
		MATROSKA_ID_CONTENTCOMPRESSION = 0x5034,
		MATROSKA_ID_CONTENTCOMPRESSION_ALGORITHM = 0x4254,
		MATROSKA_ID_CONTENTCOMPRESSION_SETTINGS = 0x4255,
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
		MATROSKA_ID_CUERELATIVEPOSITION = 0xF0,
		//Tagging
		MATROSKA_ID_TAGS = 0x1254C367,
	};

	enum ETrackType
	{
		TRACK_TYPE_VIDEO = 1,
		TRACK_TYPE_AUDIO = 2,
		TRACK_TYPE_SUBTITLE = 0x11,
	};

	//Track encoding
	enum class TrackEncodingScope
	{
		AllFrameContent = 1
	};
	enum class TrackEncodingType
	{
		Compression = 0,
	};
	enum class TrackCompressionAlgorithm
	{
		zlib = 0,
		HeaderStripping = 3
	};
	struct TrackEncoding
	{
		uint8 order = 0;
		TrackEncodingScope scope = TrackEncodingScope::AllFrameContent;
		TrackEncodingType type = TrackEncodingType::Compression;
		struct
		{
			TrackCompressionAlgorithm algorithm = TrackCompressionAlgorithm::zlib;
			DynamicArray<byte> settings;
		} compression;
	};

	//Specific codecs
#define CODEC_PCM_FLOAT_LE "A_PCM/FLOAT/IEEE"
#define CODEC_PCM_INTEGER_LE "A_PCM/INT/LIT"
	const String codecId_ms_fourcc = u8"V_MS/VFW/FOURCC";

	struct CuePointPosition : public Multimedia::TimeIndexEntryStreamInfo
	{
		uint64 trackNumber;
		uint64 clusterPos;
	};

	class CuePoint : public Multimedia::TimeIndexEntry<CuePointPosition>
	{
	};

	struct SegmentInfo
	{
		float64 duration = 0;
		uint64 timeCodeScale = 1000000;
	};

	struct Track
	{
		uint64 number;
		uint8 type;
		String codecId;
		DynamicArray<byte> codecPrivate;
		DynamicArray<TrackEncoding> encodings;
		struct
		{
			float64 sampleRate = 8000;
			uint8 nChannels = 1;
		} audio;
		struct
		{
			uint16 width = 0;
			uint16 height = 0;
		} video;
	};

	//Functions
	_stdxx_::CodingFormatIdMap<String> GetCodingFormatMap();
	void ReadCuesData(const EBML::Element &cuesElement, Multimedia::TimeIndex<CuePoint> &index, SeekableInputStream &inputStream);
	void ReadSeekHeadData(const EBML::Element &seekHead, Map<uint64, uint64> &idOffsetMap, uint64 segmentOffset, SeekableInputStream &inputStream);
	void ReadSegmentInfoData(const EBML::Element &info, SegmentInfo &segmentInfo, SeekableInputStream &inputStream);
	void ReadTrackData(const EBML::Element &tracksElement, DynamicArray<Track> &tracks, SeekableInputStream &inputStream);
}
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

void Matroska::ReadSeekHeadData(const EBML::Element &seekHead, Map<uint64, uint64> &idOffsetMap, uint64 segmentOffset, SeekableInputStream &inputStream)
{
	for(uint64 childrenSize = seekHead.dataSize; childrenSize != 0;)
	{
		EBML::Element seek;
		EBML::ParseElementHeader(seek, inputStream);
		
		ASSERT(seek.id == MATROSKA_ID_SEEK, u8"Expected seek element.");

		uint64 id = Natural<uint64>::Max(), offset = Natural<uint64>::Max();
		for (uint64 seekSize = seek.dataSize; seekSize != 0;)
		{
			EBML::Element child;
			EBML::ParseElementHeader(child, inputStream);

			switch (child.id)
			{
			case MATROSKA_ID_SEEKID: //it is defined as binary, but using UInt reading works perfectly in this case
				child.dataType = EBML::DataType::UInt;
				EBML::ReadElementData(child, inputStream);
				id = child.data.ui;
			break;
			case MATROSKA_ID_SEEKPOSITION:
				child.dataType = EBML::DataType::UInt;
				EBML::ReadElementData(child, inputStream);
				offset = child.data.ui;
				break;
			default:
				inputStream.Skip(child.dataSize);
			}
			seekSize -= child.headerSize + child.dataSize;
		}
		if ((id != Natural<uint64>::Max()) && (offset != Natural<uint64>::Max())) //only insert valid seeks
			idOffsetMap[id] = segmentOffset + offset;
		
		childrenSize -= seek.headerSize + seek.dataSize;
	}
}

void Matroska::ReadSegmentInfoData(const EBML::Element &info, SegmentInfo &segmentInfo, SeekableInputStream &inputStream)
{
	for (uint64 infoSize = info.dataSize; infoSize != 0;) //read it flattened
	{
		EBML::Element child;
		EBML::ParseElementHeader(child, inputStream);

		//set type
		switch (child.id)
		{
		case MATROSKA_ID_DURATION:
			child.dataType = EBML::DataType::Float;
			break;
		case MATROSKA_ID_TIMECODESCALE:
			child.dataType = EBML::DataType::UInt;
			break;
		}

		EBML::ReadElementData(child, inputStream);

		//set attributes
		switch (child.id)
		{
		case MATROSKA_ID_DURATION:
			segmentInfo.duration = child.data.f;
			break;
		case MATROSKA_ID_TIMECODESCALE:
			segmentInfo.timeCodeScale = child.data.ui;
			break;
		}

		if (child.dataType != EBML::DataType::Master) //flattened
			infoSize -= child.headerSize + child.dataSize;
	}
}

void Matroska::ReadTrackData(const EBML::Element &tracksElement, DynamicArray<Track> &tracks, SeekableInputStream &inputStream)
{
	for (uint64 tracksSize = tracksElement.dataSize; tracksSize != 0;)
	{
		EBML::Element trackEntry;
		EBML::ParseElementHeader(trackEntry, inputStream);
		
		ASSERT(trackEntry.id == MATROSKA_ID_TRACKENTRY, u8"Expected track entry.");

		Track track;
		for (uint64 trackEntrySize = trackEntry.dataSize; trackEntrySize != 0;)
		{
			//we read each track entry flattened
			EBML::Element child;
			EBML::ParseElementHeader(child, inputStream);

			//set type
			switch (child.id)
			{
			case MATROSKA_ID_CODECID:
				child.dataType = EBML::DataType::ASCII_String;
				break;
			case MATROSKA_ID_CODECPRIVATE:
				child.dataType = EBML::DataType::Binary;
				break;
			case MATROSKA_ID_TRACKNUMBER:
			case MATROSKA_ID_TRACKTYPE:
				child.dataType = EBML::DataType::UInt;
				break;
			}

			if(child.dataType != EBML::DataType::Binary) //handle binary ourselves here
				EBML::ReadElementData(child, inputStream);

			//set attributes
			switch (child.id)
			{
			case MATROSKA_ID_CODECID:
				track.codecId = child.data.string;
				break;
			case MATROSKA_ID_CODECPRIVATE:
				inputStream.FlushTo(track.codecPrivate, child.dataSize);
				break;
			case MATROSKA_ID_TRACKNUMBER:
				track.number = child.data.ui;
				break;
			case MATROSKA_ID_TRACKTYPE:
				track.type = child.data.ui;
				break;
			}

			trackEntrySize -= child.headerSize + child.dataSize;
		}
		tracks.Push(track);
		
		tracksSize -= trackEntry.headerSize + trackEntry.dataSize;
	}
}
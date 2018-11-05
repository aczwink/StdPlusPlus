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
//Local
#include <Std++/Streams/HashingInputStream.hpp>
#include "EBMLMasterReader.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX::Multimedia;
using namespace Matroska;

//Local functions
static void ReadContentEncoding(const EBML::Element& contentEncodingElement, InputStream& inputStream, TrackEncoding& trackEncoding)
{
	EBML::MasterReader reader(contentEncodingElement, inputStream);
	while (reader.HasMore())
	{
		EBML::Element child;
		reader.ReadNextChildHeader(child);

		//set type
		switch (child.id)
		{
		case MATROSKA_ID_CONTENTCOMPRESSION:
			child.dataType = EBML::DataType::Master;
			break;
		case MATROSKA_ID_CONTENTCOMPRESSION_ALGORITHM:
			child.dataType = EBML::DataType::UInt;
			break;
		case MATROSKA_ID_CONTENTCOMPRESSION_SETTINGS:
			child.dataType = EBML::DataType::Binary;
			break;
		}

		//set attributes
		if ((child.dataType != EBML::DataType::Binary) //handle binary ourselves here
			&& (child.dataType != EBML::DataType::Master) //we read each track entry flattened
			)
		{
			reader.ReadCurrentChildData(child);
		}

		//set attributes
		switch (child.id)
		{
		case MATROSKA_ID_CONTENTCOMPRESSION_ALGORITHM:
			trackEncoding.compression.algorithm = (TrackCompressionAlgorithm)child.data.ui;
			break;
		case MATROSKA_ID_CONTENTCOMPRESSION_SETTINGS:
		{
			trackEncoding.compression.settings.Resize(child.dataSize);
			reader.GetInUseInputStream().ReadBytes(&trackEncoding.compression.settings[0], child.dataSize);
			reader.RawBytesReadFromStream(child.dataSize);
		}
		break;
		}
	}
	reader.Verify();
}

/*
static void LoadMap()
{
	static bool loaded = false;

	if(!loaded)
	{
		loaded = true;

		//Audio codecs
		g_matroskaCodecStringMap.Insert(u8"A_MPEG/L2", CodecId::MP2);

		//Video codecs
		g_matroskaCodecStringMap.Insert(u8"V_MPEG1", CodecId::MPEG1Video);
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
	matroskaCodecMap.Insert(u8"A_AAC", CodingFormatId::AAC);
	matroskaCodecMap.Insert(u8"A_MPEG/L3", CodingFormatId::MP3);
	matroskaCodecMap.Insert(u8"A_VORBIS", CodingFormatId::Vorbis);

	//Subtitle coding formats
	matroskaCodecMap.Insert(u8"S_TEXT/UTF8", CodingFormatId::SubRip);

	//Video coding formats
	matroskaCodecMap.Insert(u8"V_MPEG4/ISO/AVC", CodingFormatId::H264);
	matroskaCodecMap.Insert(u8"V_THEORA", CodingFormatId::Theora);

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

void Matroska::ReadCuesData(const EBML::Element &cuesElement, TimeIndex<CuePoint> &index, SeekableInputStream &inputStream)
{
	EBML::MasterReader cuesReader(cuesElement, inputStream);
	while(cuesReader.HasMore())
	{
		EBML::Element cuePoint;
		cuesReader.ReadNextChildHeader(cuePoint);
		ASSERT(cuePoint.id == MATROSKA_ID_CUEPOINT, u8"Expected CuePoint element.");

		CuePoint entry;
		EBML::MasterReader cuePointReader(cuePoint, cuesReader.GetInUseInputStream());
		while(cuePointReader.HasMore())
		{
			EBML::Element child;
			cuePointReader.ReadNextChildHeader(child);
			
			//set type
			switch (child.id)
			{
			case MATROSKA_ID_CUETIME:
			case MATROSKA_ID_CUETRACK:
				child.dataType = EBML::DataType::UInt;
				break;
			case MATROSKA_ID_CUETRACKPOSITIONS:
				child.dataType = EBML::DataType::Master;
				break;
			}

			if (child.dataType != EBML::DataType::Master) //we read positions manually
				cuePointReader.ReadCurrentChildData(child);
			
			//set attributes
			switch (child.id)
			{
			case MATROSKA_ID_CUETIME:
				entry.timeStamp = child.data.ui;
				break;
			case MATROSKA_ID_CUETRACKPOSITIONS:
			{
				CuePointPosition pos;
				EBML::MasterReader cuePointPosReader(child, cuePointReader.GetInUseInputStream());
				while(cuePointPosReader.HasMore())
				{
					EBML::Element posChild;
					cuePointPosReader.ReadNextChildHeader(posChild);
					
					//set type
					switch (posChild.id)
					{
					case MATROSKA_ID_CUETRACK:
					case MATROSKA_ID_CUECLUSTERPOSITION:
						posChild.dataType = EBML::DataType::UInt;
						break;
					}

					cuePointPosReader.ReadCurrentChildData(posChild);
					
					//set attributes
					switch (posChild.id)
					{
					case MATROSKA_ID_CUETRACK:
						pos.trackNumber = posChild.data.ui;
						break;
					case MATROSKA_ID_CUECLUSTERPOSITION:
						pos.clusterPos = posChild.data.ui;
						break;
					}
				}
				cuePointPosReader.Verify();
				entry.streamsInfo.Push(pos);

				cuePointReader.RawBytesReadFromStream(child.dataSize);
			}
			break;
			}
		}
		cuePointReader.Verify();
		index.AddEntry(entry);

		cuesReader.RawBytesReadFromStream(cuePoint.dataSize);
	}
	cuesReader.Verify();
}

void Matroska::ReadSeekHeadData(const EBML::Element &seekHead, Map<uint64, uint64> &idOffsetMap, uint64 segmentOffset, SeekableInputStream &inputStream)
{
	EBML::MasterReader seekHeadReader(seekHead, inputStream);
	while (seekHeadReader.HasMore())
	{
		EBML::Element seek;
		seekHeadReader.ReadNextChildHeader(seek);
		ASSERT(seek.id == MATROSKA_ID_SEEK, u8"Expected seek element.");

		uint64 id = Natural<uint64>::Max(), offset = Natural<uint64>::Max();
		EBML::MasterReader seekReader(seek, seekHeadReader.GetInUseInputStream());
		while(seekReader.HasMore())
		{
			EBML::Element child;
			seekReader.ReadNextChildHeader(child);
			
			switch (child.id)
			{
			case MATROSKA_ID_SEEKID: //it is defined as binary, but using UInt reading works perfectly in this case
				child.dataType = EBML::DataType::UInt;
				seekReader.ReadCurrentChildData(child);
				id = child.data.ui;
			break;
			case MATROSKA_ID_SEEKPOSITION:
				child.dataType = EBML::DataType::UInt;
				seekReader.ReadCurrentChildData(child);
				offset = child.data.ui;
				break;
			default:
				seekReader.SkipCurrentChildData(child);
			}
		}
		seekReader.Verify();
		if ((id != Natural<uint64>::Max()) && (offset != Natural<uint64>::Max())) //only insert valid seeks
			idOffsetMap[id] = segmentOffset + offset;
		
		seekHeadReader.RawBytesReadFromStream(seek.dataSize);
	}
	seekHeadReader.Verify();
}

void Matroska::ReadSegmentInfoData(const EBML::Element &info, SegmentInfo &segmentInfo, SeekableInputStream &inputStream)
{
	EBML::MasterReader reader(info, inputStream);
	while(reader.HasMore())
	{
		EBML::Element child;
		reader.ReadNextChildHeader(child);
		
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

		reader.ReadCurrentChildData(child);
		
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
	}
	reader.Verify();
}

void Matroska::ReadTrackData(const EBML::Element &tracksElement, DynamicArray<Track> &tracks, SeekableInputStream &inputStream)
{
	EBML::MasterReader reader(tracksElement, inputStream);
	while(reader.HasMore())
	{
		EBML::Element trackEntry;
		reader.ReadNextChildHeader(trackEntry);
		ASSERT(trackEntry.id == MATROSKA_ID_TRACKENTRY, u8"Expected track entry.");

		Track track;
		EBML::MasterReader trackReader(trackEntry, reader.GetInUseInputStream());
		while(trackReader.HasMore())
		{
			EBML::Element child;
			trackReader.ReadNextChildHeader(child);

			//set type
			switch (child.id)
			{
			case MATROSKA_ID_AUDIO:
			case MATROSKA_ID_CONTENTENCODING:
			case MATROSKA_ID_CONTENTENCODINGS:
			case MATROSKA_ID_VIDEO:
				child.dataType = EBML::DataType::Master;
				break;
			case MATROSKA_ID_CODECID:
				child.dataType = EBML::DataType::ASCII_String;
				break;
			case MATROSKA_ID_CODECPRIVATE:
				child.dataType = EBML::DataType::Binary;
				break;
			case MATROSKA_ID_SAMPLINGFREQUENCY:
				child.dataType = EBML::DataType::Float;
				break;
			case MATROSKA_ID_CHANNELS:
			case MATROSKA_ID_PIXELHEIGHT:
			case MATROSKA_ID_PIXELWIDTH:
			case MATROSKA_ID_TRACKNUMBER:
			case MATROSKA_ID_TRACKTYPE:
				child.dataType = EBML::DataType::UInt;
				break;
			}

			if ((child.dataType != EBML::DataType::Binary) //handle binary ourselves here
				&& (child.dataType != EBML::DataType::Master) //we read each track entry flattened
				)
			{
				trackReader.ReadCurrentChildData(child);
			}

			//set attributes
			switch (child.id)
			{
			case MATROSKA_ID_CHANNELS:
				track.audio.nChannels = child.data.ui;
				break;
			case MATROSKA_ID_CODECID:
				track.codecId = child.data.string;
				break;
			case MATROSKA_ID_CODECPRIVATE:
				track.codecPrivate.Resize(child.dataSize);
				trackReader.GetInUseInputStream().ReadBytes(&track.codecPrivate[0], child.dataSize);
				trackReader.RawBytesReadFromStream(child.dataSize);
				break;
			case MATROSKA_ID_CONTENTENCODING:
			{
				TrackEncoding encoding;
				ReadContentEncoding(child, trackReader.GetInUseInputStream(), encoding);
				track.encodings.Push(encoding);

				trackReader.RawBytesReadFromStream(child.dataSize);
			}
			break;
			case MATROSKA_ID_PIXELHEIGHT:
				track.video.height = child.data.ui;
				break;
			case MATROSKA_ID_PIXELWIDTH:
				track.video.width = child.data.ui;
				break;
			case MATROSKA_ID_SAMPLINGFREQUENCY:
				track.audio.sampleRate = child.data.f;
				break;
			case MATROSKA_ID_TRACKNUMBER:
				track.number = child.data.ui;
				break;
			case MATROSKA_ID_TRACKTYPE:
				track.type = child.data.ui;
				break;
			}
		}
		trackReader.Verify();
		tracks.Push(track);

		reader.RawBytesReadFromStream(trackEntry.dataSize);
	}
	reader.Verify();
}
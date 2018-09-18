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
//Class header
#include "MatroskaDemuxer.hpp"
//Local
#include <Std++/Multimedia/AudioStream.hpp>
#include <Std++/Multimedia/SubtitleStream.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>
#include "EBML.hpp"
#include "Matroska.hpp"
#include "../BMP/BMP.hpp"
//Namespaces
using namespace Matroska;

//Constructor
MatroskaDemuxer::MatroskaDemuxer(const Format &refFormat, SeekableInputStream &refInput) : Demuxer(refFormat, refInput), codecIdMap(GetCodingFormatMap())
{
	this->timeScale = Fraction(1000000, 1000000000); //default timeCodeScale is 1.000.000 but all units are in nanoseconds
}

//Private methods
/*
bool MatroskaDemuxer::GetElementInfo(uint64 id, SElemInfo &refElemInfo)
{
	switch (id)
	{
	case MATROSKA_ID_BLOCK:
	case MATROSKA_ID_SIMPLEBLOCK:
		refElemInfo.type = EMatroskaType::Binary;
		return true;
	case MATROSKA_ID_SAMPLINGFREQUENCY:
		refElemInfo.type = EMatroskaType::Float;
		return true;
	case MATROSKA_ID_AUDIO:
		refElemInfo.type = EMatroskaType::Master;
		return true;
	case MATROSKA_ID_BITDEPTH:
	case MATROSKA_ID_CHANNELS:
	case MATROSKA_ID_TIMECODE:
		refElemInfo.type = EMatroskaType::UInt;
		return true;
	}

	return false;
}

void MatroskaDemuxer::ParseBinary(uint64 id, uint64 size)
{
	switch(id)
	{
		case MATROSKA_ID_BLOCK:
		case MATROSKA_ID_SIMPLEBLOCK:
		{
			this->parserState.currentCluster.offsets.Push(this->inputStream.GetCurrentOffset());
			this->parserState.currentCluster.sizes.Push(size);

			this->inputStream.Skip(size);
		}
			break;
	}
}

void MatroskaDemuxer::ParseFloat(uint64 id, float64 value)
{
	switch(id)
	{
		case MATROSKA_ID_SAMPLINGFREQUENCY:
			this->parserState.currentTrack.audio.samplingFrequency = value;
			break;
	}
}

void MatroskaDemuxer::ParseUInt(uint64 id, uint64 value)
{
	switch(id)
	{
		case MATROSKA_ID_BITDEPTH:
			this->parserState.currentTrack.audio.bitDepth = (uint8)value;
			break;
		case MATROSKA_ID_CHANNELS:
			this->parserState.currentTrack.audio.nChannels = (uint8)value;
			break;
		case MATROSKA_ID_TIMECODE:
			this->parserState.currentCluster.timeCode = value;
			break;
	}
}*/

//Public methods
void MatroskaDemuxer::ReadHeader()
{
	//we don't need to check header, because this was done in the container format
	EBML::Header header;
	EBML::ParseHeader(header, this->inputStream);
	
	//read segments
	DynamicArray<uint64> segmentOffsets;
	while(!this->inputStream.IsAtEnd())
	{
		EBML::Element element;
		EBML::ParseElementHeader(element, this->inputStream);
		ASSERT(element.id == MATROSKA_ID_SEGMENT, u8"Invalid element found in matroska file. Expected segment.");
		segmentOffsets.Push(element.dataOffset);
		this->inputStream.Skip(element.dataSize);
	}

	//read segment
	ASSERT(segmentOffsets.GetNumberOfElements() == 1, u8"Can't read matroska files with multiple segments currently.");

	this->ReadSegment(segmentOffsets[0]);

	//ReadSegment will place us at the beginning of the first cluster
}

bool MatroskaDemuxer::ReadPacket(Packet &packet)
{
	DataReader reader(true, this->inputStream);

	if (this->demuxerState.lacedFrameSizes.IsEmpty())
	{
		//read new cluster or block
		bool foundBlock = false;
		uint32 blockSize;
		while (!foundBlock)
		{
			EBML::Element element;
			EBML::ParseElementHeader(element, this->inputStream);

			if (this->inputStream.IsAtEnd())
				return false; //no more data

			switch (element.id)
			{
			case MATROSKA_ID_CLUSTER:
				//do nothing
				break;
			case MATROSKA_ID_TIMECODE:
				element.dataType = EBML::DataType::UInt;
				EBML::ReadElementData(element, this->inputStream);
				this->demuxerState.clusterTimecode = element.data.ui;
				break;
			case MATROSKA_ID_SIMPLEBLOCK:
				foundBlock = true;
				blockSize = element.dataSize;
				break;
			default:
				this->inputStream.Skip(element.dataSize);
			}
		}

		//read block header
		uint8 length;
		uint64 trackNumber = EBML::DecodeVariableLengthInteger(length, this->inputStream);
		int16 timeCode = reader.ReadInt16();
		uint8 flags = reader.ReadByte();
		uint32 blockHeaderSize = length + 2 + 1;

		this->demuxerState.blockStreamIndex = this->trackToStreamMap[trackNumber];
		switch ((flags >> 1) & 3)
		{
		case 0: //no lacing
		{
			packet.Allocate(blockSize - blockHeaderSize);
			this->inputStream.ReadBytes(packet.GetData(), packet.GetSize());

			packet.streamIndex = this->demuxerState.blockStreamIndex;
			packet.pts = this->demuxerState.clusterTimecode + timeCode;
			packet.containsKeyframe = (flags & 0x80) != 0; //TODO: this assumes that this is a SimpleBlock. It will fail for standard blocks as this will always be 0

			return true;
		}
		break;
		case 2: //fixed-size lacing
		{
			uint8 nFrames = reader.ReadByte() + 1;
			blockHeaderSize++;

			uint32 frameSize = (blockSize - blockHeaderSize) / nFrames;
			for (uint8 i = 0; i < nFrames; i++)
				this->demuxerState.lacedFrameSizes.InsertTail(frameSize);
		}
		break;
		case 3: //EBML lacing
		{
			uint8 nEncodedLaceSizes = reader.ReadByte();
			blockHeaderSize++;
			uint64 laceSize = EBML::DecodeVariableLengthInteger(length, this->inputStream);
			blockHeaderSize += length;

			this->demuxerState.lacedFrameSizes.InsertTail(laceSize);
			nEncodedLaceSizes--;

			uint64 sum = laceSize;
			for (uint8 i = 0; i < nEncodedLaceSizes; i++)
			{
				uint64 codedSize = EBML::DecodeVariableLengthInteger(length, this->inputStream);
				blockHeaderSize += length;
				int64 rangeShift = (1 << (length * 8 - length - 1)) - 1;
				laceSize += int64(codedSize) - rangeShift;
				this->demuxerState.lacedFrameSizes.InsertTail(laceSize);

				sum += laceSize;
			}

			this->demuxerState.lacedFrameSizes.InsertTail(blockSize - blockHeaderSize - sum);
		}
		break;
		default:
			NOT_IMPLEMENTED_ERROR;
		}
	}
	
	//read next frame from lace
	uint64 frameSize = this->demuxerState.lacedFrameSizes.PopFront();
	packet.Allocate(frameSize);
	this->inputStream.ReadBytes(packet.GetData(), packet.GetSize());
	
	packet.streamIndex = this->demuxerState.blockStreamIndex;

	return true;
}

//Private methods
void MatroskaDemuxer::AddStream(Matroska::Track &track)
{
	//create stream
	Stream *pStream;
	switch(track.type)
	{
	case TRACK_TYPE_AUDIO:
		pStream = new AudioStream;
		break;
	case TRACK_TYPE_SUBTITLE:
		pStream = new SubtitleStream;
		break;
	case TRACK_TYPE_VIDEO:
		pStream = new VideoStream;
		break;
	default:
		NOT_IMPLEMENTED_ERROR;
		pStream = nullptr;
	}
	
	pStream->timeScale = this->timeScale;
	CodingFormatId codingFormatId = this->codecIdMap.Get(track.codecId);
	pStream->SetCodingFormat(codingFormatId);
	
	uint32 index = Demuxer::AddStream(pStream);
	this->trackToStreamMap[track.number] = index;
	
	//stream specific
	switch(track.type)
	{
	case TRACK_TYPE_AUDIO:
	{
		AudioStream *const& refpAudioStream = (AudioStream *)pStream;

		/*		
		refpAudioStream->nChannels = this->parserState.currentTrack.audio.nChannels;
		refpAudioStream->sampleRate = (uint32)this->parserState.currentTrack.audio.samplingFrequency;
		*/
		
		/*
		//check for PCM
		if(this->parserState.currentTrack.codingFormatId == CodingFormatId::Unknown && this->parserState.currentTrack.codecPrivate.isFloatPCM)
		{
		switch(this->parserState.currentTrack.audio.bitDepth)
		{
		case 32:
		pStream->SetCodingFormat(CodingFormatId::PCM_Float32LE);
		break;
		default:
		NOT_IMPLEMENTED_ERROR;
		}
		}*/
	}
	break;
	case TRACK_TYPE_VIDEO:
	{
		VideoStream *const& refpVideoStream = (VideoStream *)pStream;

		//check for microsoft BMP header
		if (codingFormatId == CodingFormatId::Unknown && track.codecId == codecId_ms_fourcc)
		{
			bool isBottomUp;
			_stdxx_::ReadBMPHeader(isBottomUp, track.codecPrivate, *refpVideoStream);
		}
	}
	break;
	}
}

void MatroskaDemuxer::ReadSegment(uint64 segmentOffset)
{
	this->inputStream.SetCurrentOffset(segmentOffset);

	Map<uint64, uint64> idOffsetMap;
	FiniteSet<uint64> readSections;
	bool foundCluster = false;
	while (!foundCluster)
	{
		EBML::Element element;
		EBML::ParseElementHeader(element, this->inputStream);

		switch (element.id)
		{
		case MATROSKA_ID_SEEKHEAD:
			Matroska::ReadSeekHeadData(element, idOffsetMap, segmentOffset, this->inputStream);
			break;
		case MATROSKA_ID_CUES:
		case MATROSKA_ID_INFO:
		case MATROSKA_ID_TRACKS:
			this->ReadSection(element);
			readSections.Insert(element.id);
			break;
		case MATROSKA_ID_CLUSTER:
			foundCluster = true;
			break; //we are at the clusters.... break from here
		default: //unknown... skip
			readSections.Insert(element.id);
			this->inputStream.Skip(element.dataSize);
		}
	}

	for (const auto &kv : idOffsetMap)
	{
		if (!readSections.Contains(kv.key))
		{
			uint64 currentOffset = this->inputStream.GetCurrentOffset();

			this->inputStream.SetCurrentOffset(kv.value);
			EBML::Element element;
			EBML::ParseElementHeader(element, this->inputStream);
			this->ReadSection(element);

			this->inputStream.SetCurrentOffset(currentOffset);
		}
	}

	ASSERT(foundCluster, u8"if you see this, report this please.")
}

void MatroskaDemuxer::ReadSection(const EBML::Element &element)
{
	switch (element.id)
	{
	case MATROSKA_ID_CUES:
	{
		Matroska::ReadCuesData(element, this->cues, this->inputStream);
	}
	break;
	case MATROSKA_ID_INFO:
	{
		Matroska::SegmentInfo segmentInfo;
		Matroska::ReadSegmentInfoData(element, segmentInfo, this->inputStream);

		if(segmentInfo.duration != 0)
			this->duration = (uint64)segmentInfo.duration;

		this->timeScale.numerator = segmentInfo.timeCodeScale;
		for (uint32 i = 0; i < this->GetNumberOfStreams(); i++)
			this->GetStream(i)->timeScale = this->timeScale;
	}
	break;
	case MATROSKA_ID_TRACKS:
	{
		DynamicArray<Matroska::Track> tracks;
		Matroska::ReadTrackData(element, tracks, this->inputStream);

		for (Matroska::Track &track : tracks)
			this->AddStream(track);
	}
	break;
	default:
		this->inputStream.Skip(element.dataSize);
	}
}
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
void MatroskaDemuxer::AddStream()
{
	uint32 index;
	Stream *pStream;

	//create stream
	switch(this->parserState.currentTrack.type)
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
	pStream->SetCodingFormat(this->parserState.currentTrack.codingFormatId);

	index = Demuxer::AddStream(pStream);
	this->trackToStreamMap[this->parserState.currentTrack.number] = index;

	//stream specific
	switch(this->parserState.currentTrack.type)
	{
		case TRACK_TYPE_AUDIO:
		{
			AudioStream *const& refpAudioStream = (AudioStream *)pStream;

			refpAudioStream->nChannels = this->parserState.currentTrack.audio.nChannels;
			refpAudioStream->sampleRate = (uint32)this->parserState.currentTrack.audio.samplingFrequency;

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
			if(this->parserState.currentTrack.codingFormatId == CodingFormatId::Unknown && this->parserState.currentTrack.codecPrivate.isMS_BMPHeader)
			{
				bool isBottomUp;
				uint64 currentOffset;

				currentOffset = this->inputStream.GetCurrentOffset();
				this->inputStream.SetCurrentOffset(this->parserState.currentTrack.codecPrivate.offset);
				_stdxx_::ReadBMPHeader(isBottomUp, this->inputStream, *refpVideoStream);
				this->inputStream.SetCurrentOffset(currentOffset);
			}
		}
			break;
	}
}

/*void MatroskaDemuxer::BeginParseChilds(uint64 id)
{
	switch(id)
	{
		case MATROSKA_ID_SEGMENT:
		{
		}
			break;
		case MATROSKA_ID_TRACKENTRY:
		{
			this->parserState.currentTrack.type = Natural<uint8>::Max();
			this->parserState.currentTrack.codingFormatId = CodingFormatId::Unknown;
			this->parserState.currentTrack.codecPrivate.isMS_BMPHeader = false;
			this->parserState.currentTrack.codecPrivate.isFloatPCM = false;
			this->parserState.currentTrack.codecPrivate.isIntegerPCM = false;
		}
			break;
	}
}

void MatroskaDemuxer::EndParseChilds(uint64 id)
{
	switch(id)
	{
		case MATROSKA_ID_CLUSTER:
		{
			for(uint32 i = 0; i < this->parserState.currentCluster.offsets.GetNumberOfElements(); i++)
			{
				this->clusters.AddCluster(this->parserState.currentCluster.offsets[i], this->parserState.currentCluster.sizes[i], this->parserState.currentCluster.timeCode);
			}

			this->parserState.currentCluster.offsets.Clear();
			this->parserState.currentCluster.sizes.Clear();
		}
			break;
		case MATROSKA_ID_TRACKENTRY:
			this->AddStream();
			break;
	}
}

bool MatroskaDemuxer::GetElementInfo(uint64 id, SElemInfo &refElemInfo)
{
	switch (id)
	{
	case MATROSKA_ID_CODECID:
		refElemInfo.type = EMatroskaType::ASCII_String;
		return true;
	case MATROSKA_ID_BLOCK:
	case MATROSKA_ID_CODECPRIVATE:
	case MATROSKA_ID_SIMPLEBLOCK:
		refElemInfo.type = EMatroskaType::Binary;
		return true;
	case MATROSKA_ID_DURATION:
	case MATROSKA_ID_SAMPLINGFREQUENCY:
		refElemInfo.type = EMatroskaType::Float;
		return true;
	case MATROSKA_ID_AUDIO:
	case MATROSKA_ID_CLUSTER:
	case MATROSKA_ID_INFO:
	case MATROSKA_ID_SEGMENT:
	case MATROSKA_ID_TRACKS:
	case MATROSKA_ID_TRACKENTRY:
		refElemInfo.type = EMatroskaType::Master;
		return true;
	case MATROSKA_ID_BITDEPTH:
	case MATROSKA_ID_CHANNELS:
	case MATROSKA_ID_TIMECODE:
	case MATROSKA_ID_TIMECODESCALE:
	case MATROSKA_ID_TRACKNUMBER:
	case MATROSKA_ID_TRACKTYPE:
		refElemInfo.type = EMatroskaType::UInt;
		return true;
	}

	return false;
}

void MatroskaDemuxer::ParseASCIIString(uint64 id, const String &string)
{
	switch(id)
	{
	case MATROSKA_ID_CODECID:
	{
		this->parserState.currentTrack.codingFormatId = this->codecIdMap.Get(string);

		//special cases
		if (string == codecId_ms_fourcc)
			this->parserState.currentTrack.codecPrivate.isMS_BMPHeader = true;
	}
	break;
	}
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
		case MATROSKA_ID_CODECPRIVATE:
		{
			this->parserState.currentTrack.codecPrivate.offset = this->inputStream.GetCurrentOffset();
			this->parserState.currentTrack.codecPrivate.size = size;

			this->inputStream.Skip(size);
		}
			break;
		default:
			this->inputStream.Skip(size);
	}
}

void MatroskaDemuxer::ParseFloat(uint64 id, float64 value)
{
	switch(id)
	{
		case MATROSKA_ID_DURATION:
			this->duration = (uint64)value;
			break;
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
		case MATROSKA_ID_TRACKNUMBER:
			this->parserState.currentTrack.number = value;
			break;
		case MATROSKA_ID_TRACKTYPE:
			this->parserState.currentTrack.type = (uint8)value;
			break;
		case MATROSKA_ID_TIMECODE:
			this->parserState.currentCluster.timeCode = value;
			break;
		case MATROSKA_ID_TIMECODESCALE:
		{
			this->timeScale.numerator = value;

			for(uint32 i = 0; i < this->GetNumberOfStreams(); i++)
			{
				this->GetStream(i)->timeScale = this->timeScale;
			}
		}
			break;
	}
}*/

//Public methods
void MatroskaDemuxer::ReadHeader()
{
	EBMLParser parser(this->inputStream);
	while(!this->inputStream.IsAtEnd())
		this->ParseElement();

	//move to beginning of data
	this->inputStream.SetCurrentOffset(this->clusters.GetStartOffset());
}

bool MatroskaDemuxer::ReadPacket(Packet &packet)
{
	uint32 clusterIndex;

	DataReader reader(true, this->inputStream);

	//find cluster
	while (true)
	{
		if (this->clusters.FindEntry(this->inputStream.GetCurrentOffset(), clusterIndex))
			break;
		
		//resync
		const CClusterEntry &refEntry = this->clusters.GetCluster(clusterIndex);
		
		if (refEntry.offset < this->inputStream.GetCurrentOffset())
			clusterIndex++; //go to next cluster
		if (clusterIndex >= this->clusters.GetNumberOfClusters())
			return false; //read all clusters... we're at the end
						  
		//move input stream to next cluster offset
		const CClusterEntry &refResyncEntry = this->clusters.GetCluster(clusterIndex);
		this->inputStream.SetCurrentOffset(refResyncEntry.offset);
	}
	const CClusterEntry &cluster = this->clusters.GetCluster(clusterIndex);

	//are we at the beginning of a lace or block header
	if (this->demuxerState.lacedFrameSizes.IsEmpty())
	{
		//read next block header
		uint8 length;
		uint64 trackNumber = this->DecodeVariableLengthInteger(length);
		int16 timeCode = reader.ReadInt16();
		uint8 flags = reader.ReadByte();

		this->demuxerState.blockStreamIndex = this->trackToStreamMap[trackNumber];

		switch ((flags >> 1) & 3)
		{
		case 0: //no lacing
		{
			packet.Allocate(cluster.GetRemainingBytes(this->inputStream.GetCurrentOffset()));
			this->inputStream.ReadBytes(packet.GetData(), packet.GetSize());

			packet.streamIndex = this->demuxerState.blockStreamIndex;
			packet.pts = cluster.timeStamp + timeCode;
			packet.containsKeyframe = (flags & 0x80) != 0; //TODO: this assumes that this is a SimpleBlock. It will fail for standard blocks as this will always be 0

			return true;
		}
		break;
		case 3: //EBML lacing
		{
			uint8 nEncodedLaceSizes = reader.ReadByte();
			uint64 laceSize = this->DecodeVariableLengthInteger(length);

			this->demuxerState.lacedFrameSizes.InsertTail(laceSize);
			nEncodedLaceSizes--;

			uint64 sum = laceSize;
			for (uint8 i = 0; i < nEncodedLaceSizes; i++)
			{

				uint64 codedSize = this->DecodeVariableLengthInteger(length);
				int64 rangeShift = (1 << (length * 8 - length - 1)) - 1;
				laceSize += int64(codedSize) - rangeShift;
				this->demuxerState.lacedFrameSizes.InsertTail(laceSize);

				sum += laceSize;
			}
			
			this->demuxerState.lacedFrameSizes.InsertTail(cluster.size - sum); //TODO: cluster is actually implemented as block index! this is wrong! this should say block size - sum
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
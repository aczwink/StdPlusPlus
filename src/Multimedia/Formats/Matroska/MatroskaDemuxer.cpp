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
#include "../BMP/BMP.h"
//Namespaces
using namespace Matroska;

//Constructor
MatroskaDemuxer::MatroskaDemuxer(const Format &refFormat, SeekableInputStream &refInput) : Demuxer(refFormat, refInput)
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
	pStream->SetCodec(this->parserState.currentTrack.codecId);

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

			//check for PCM
			if(this->parserState.currentTrack.codecId == CodecId::Unknown && this->parserState.currentTrack.codecPrivate.isFloatPCM)
			{
				switch(this->parserState.currentTrack.audio.bitDepth)
				{
					case 32:
						pStream->SetCodec(CodecId::PCM_Float32LE);
						break;
					default:
						NOT_IMPLEMENTED_ERROR;
				}
			}
		}
			break;
		case TRACK_TYPE_VIDEO:
		{
			VideoStream *const& refpVideoStream = (VideoStream *)pStream;

			//check for microsoft BMP header
			if(this->parserState.currentTrack.codecId == CodecId::Unknown && this->parserState.currentTrack.codecPrivate.isMS_BMPHeader)
			{
				bool isBottomUp;
				uint64 currentOffset;

				currentOffset = this->inputStream.GetCurrentOffset();
				this->inputStream.SetCurrentOffset(this->parserState.currentTrack.codecPrivate.offset);
				ReadBMPHeader(isBottomUp, this->inputStream, *refpVideoStream);
				this->inputStream.SetCurrentOffset(currentOffset);
			}
		}
			break;
	}
}

void MatroskaDemuxer::BeginParseChilds(uint64 id)
{
	switch(id)
	{
		case MATROSKA_ID_SEGMENT:
		{
		}
			break;
		case MATROSKA_ID_TRACKENTRY:
		{
			this->parserState.currentTrack.type = UINT8_MAX;
			this->parserState.currentTrack.codecId = CodecId::Unknown;
			this->parserState.currentTrack.codecPrivate.isMS_BMPHeader = false;
			this->parserState.currentTrack.codecPrivate.isFloatPCM = false;
			this->parserState.currentTrack.codecPrivate.isIntegerPCM = false;
		}
			break;
	}
}

uint64 MatroskaDemuxer::DecodeVariableLengthInteger(uint8 &refLength)
{
	byte first, mask;
	uint64 result;

	DataReader reader(true, this->inputStream);

	result = 0;
	refLength = 1;
	mask = 0x80;
	first = reader.ReadByte();
	while(mask)
	{
		if(first & mask)
		{
			uint8 firstBitLength, restBitLength;

			firstBitLength = 8 - refLength;
			restBitLength = (refLength - 1) * 8;

			first &= (1 << firstBitLength) - 1;
			return result | ((uint64)first << restBitLength);
		}

		result = (result << 8) | reader.ReadByte();
		refLength++;
		mask >>= 1;
	}

	NOT_IMPLEMENTED_ERROR;
	return result;
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
	switch(id)
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

CodecId MatroskaDemuxer::MapCodecId(const ByteString &refString)
{
	//audio codecs
	if(refString == CODEC_PCM_FLOAT_LE)
	{
		this->parserState.currentTrack.codecPrivate.isFloatPCM = true;
		return CodecId::Unknown;
	}

	if(refString == CODEC_PCM_INTEGER_LE)
	{
		this->parserState.currentTrack.codecPrivate.isIntegerPCM = true;
		return CodecId::Unknown;
	}

	//video codecs
	if(refString == CODEC_MS_FOURCC)
	{
		this->parserState.currentTrack.codecPrivate.isMS_BMPHeader = true;
		return CodecId::Unknown;
	}

	return MapCodecString(refString);
}

void MatroskaDemuxer::ParseASCIIString(uint64 id, const ByteString &refString)
{
	switch(id)
	{
		case MATROSKA_ID_CODECID:
		{
			this->parserState.currentTrack.codecId = this->MapCodecId(refString);
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

uint64 MatroskaDemuxer::ParseElement()
{
	uint8 headerLength, length;
	uint64 id, size;
	SElemInfo elemInfo;

	DataReader reader(true, this->inputStream);

	//read id
	id = this->DecodeVariableLengthInteger(length);
	id = PutLength(id, length);

	headerLength = length;

	//read size
	size = this->DecodeVariableLengthInteger(length);

	headerLength += length;

	if(this->inputStream.IsAtEnd())
		return 0;

	if(this->GetElementInfo(id, elemInfo))
	{
		switch(elemInfo.type)
		{
			case EMatroskaType::ASCII_String:
			{
				ByteString str;

				TextReader reader(this->inputStream, TextCodecType::ASCII);

				NOT_IMPLEMENTED_ERROR; //TODO: needs new string class
				//str = reader.ReadString(size);
				this->ParseASCIIString(id, str);
			}
				break;
			case EMatroskaType::Binary:
			{
				this->ParseBinary(id, size);
			}
				break;
			case EMatroskaType::Date:
			{
				this->inputStream.Skip(size);
			}
				break;
			case EMatroskaType::Float:
			{
				float64 value;

				if(size == 4)
					value = reader.ReadFloat32();
				else
					value = reader.ReadFloat64();

				this->ParseFloat(id, value);
			}
				break;
			case EMatroskaType::Master:
			{
				uint64 left;

				this->BeginParseChilds(id);

				left = size;
				while(left)
				{
					left -= this->ParseElement();
				}

				this->EndParseChilds(id);
			}
				break;
			case EMatroskaType::UInt:
			{
				uint8 left;
				uint64 value;

				value = 0;
				left = (uint8)size;
				while(left--)
					value = (value << 8) | reader.ReadByte();

				this->ParseUInt(id, value);
			}
				break;
			case EMatroskaType::UTF8:
			{
				this->inputStream.Skip(size);
			}
				break;
			default:
				NOT_IMPLEMENTED_ERROR;
		}
	}
	else
	{
		this->inputStream.Skip(size);
	}

	return headerLength + size;
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
}

//Public methods
void MatroskaDemuxer::ReadHeader()
{
	while(!this->inputStream.IsAtEnd())
		this->ParseElement();

	//move to beginning of data
	this->inputStream.SetCurrentOffset(this->clusters.GetStartOffset());
}

bool MatroskaDemuxer::ReadPacket(Packet &packet)
{
	uint32 clusterIndex;

	DataReader reader(true, this->inputStream);

	while(true)
	{
		if(this->clusters.FindEntry(this->inputStream.GetCurrentOffset(), clusterIndex))
		{
			uint8 length, flags;
			int16 timeCode;
			uint64 trackNumber;

			const CClusterEntry &refEntry = this->clusters.GetCluster(clusterIndex);

			trackNumber = this->DecodeVariableLengthInteger(length);
			timeCode = reader.ReadInt16();
			flags = reader.ReadByte();

			switch((flags >> 5) & 3)
			{
				case 0: //no lacing
				{
					packet.Allocate(refEntry.GetRemainingBytes(this->inputStream.GetCurrentOffset()));
					this->inputStream.ReadBytes(packet.GetData(), packet.GetSize());

					packet.streamIndex = this->trackToStreamMap[trackNumber];
					packet.pts = refEntry.timeStamp + timeCode;
					packet.containsKeyframe = (flags & 0x80) != 0;

					return true;
				}
					break;
				default:
					NOT_IMPLEMENTED_ERROR;
			}
		}
		else
		{
			//resync
			const CClusterEntry &refEntry = this->clusters.GetCluster(clusterIndex);

			if(refEntry.offset < this->inputStream.GetCurrentOffset())
				clusterIndex++; //go to next cluster
			if(clusterIndex >= this->clusters.GetNumberOfClusters())
				break; //read all clusters... we're at the end

			//move input stream to next cluster offset
			const CClusterEntry &refResyncEntry = this->clusters.GetCluster(clusterIndex);

			this->inputStream.SetCurrentOffset(refResyncEntry.offset);
		}
	}

	return false;
}
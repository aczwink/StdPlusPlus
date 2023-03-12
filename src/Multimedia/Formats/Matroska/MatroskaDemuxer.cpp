/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/Readers/TextReader.hpp>
#include <Std++/Multimedia/CodingParameters.hpp>
#include "EBML.hpp"
#include "EBMLMasterReader.hpp"
#include "Matroska.hpp"
#include "../BMP/BMP.hpp"
//Namespaces
using namespace Matroska;

//Constructor
MatroskaDemuxer::MatroskaDemuxer(const Format &refFormat, SeekableInputStream &refInput) : Demuxer(refFormat, refInput), codecIdMap(GetCodingFormatMap())
{
	this->timeScale = {1000000, 1000000000}; //default timeCodeScale is 1.000.000 but all units are in nanoseconds
}

//Private methods
/*
bool MatroskaDemuxer::GetElementInfo(uint64 id, SElemInfo &refElemInfo)
{
	switch (id)
	{
	case MATROSKA_ID_BITDEPTH:
		refElemInfo.type = EMatroskaType::UInt;
		return true;
	}

	return false;
}

void MatroskaDemuxer::ParseUInt(uint64 id, uint64 value)
{
	switch(id)
	{
		case MATROSKA_ID_BITDEPTH:
			this->parserState.currentTrack.audio.bitDepth = (uint8)value;
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
		
		if (element.SizeUnknown()) //live recording, there can only be one segment
		{
			this->ReadSegment(element.dataOffset, true);
			return;
		}
		
		this->inputStream.Skip(element.dataSize);
	}

	//read segment
	ASSERT(segmentOffsets.GetNumberOfElements() == 1, u8"Can't read matroska files with multiple segments currently.");

	this->inputStream.SeekTo(segmentOffsets[0]);
	this->ReadSegment(segmentOffsets[0], false);

	//ReadSegment will place us at the beginning of the first cluster
}

void MatroskaDemuxer::Seek(uint64 timestamp, const class TimeScale &timeScale)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

UniquePointer<IPacket> MatroskaDemuxer::ReadPacket()
{
	while (this->demuxerState.packetQueue.IsEmpty())
	{
		//read new cluster or block
		EBML::Element element;
		EBML::ParseElementHeader(element, this->inputStream);

		if (this->inputStream.IsAtEnd())
			return nullptr; //no more data
		
		switch (element.id)
		{
		case MATROSKA_ID_CLUSTER:
			this->demuxerState.leftClusterSize = element.dataSize;
			//do nothing
			break;
		case MATROSKA_ID_TIMECODE:
			element.dataType = EBML::DataType::UInt;
			EBML::ReadElementData(element, this->inputStream);
			this->demuxerState.clusterTimecode = element.data.ui;

			this->demuxerState.leftClusterSize -= element.Size();
			break;
		case MATROSKA_ID_BLOCKGROUP:
		{
			EBML::MasterReader reader(element, this->inputStream);
			Optional<uint64> duration;
			bool isKeyframe = true;
			while (reader.HasMore())
			{
				EBML::Element child;
				reader.ReadNextChildHeader(child);

				this->demuxerState.leftClusterSize -= child.headerSize;

				switch (child.id)
				{
				case MATROSKA_ID_BLOCK:
				{
					uint8 blockHeaderSize = this->ReadBlockHeader(false, child.dataSize);
					reader.RawBytesReadFromStream(blockHeaderSize);
					uint32 blockDataSize = child.dataSize - blockHeaderSize;
					if (reader.GetRemainingBytes() > blockDataSize)
					{
						this->BufferPackets();
						reader.RawBytesReadFromStream(blockDataSize);
					}
					else
					{
						//no need to buffer packets...
						reader.RawBytesReadFromStream(reader.GetRemainingBytes()); //we fool the reader and tell him as if we consumed the whole block data
						//though stream is actually at beginning of block payload
					}

					this->demuxerState.leftClusterSize -= blockHeaderSize + blockDataSize;
				}
				break;
				case MATROSKA_ID_BLOCKDURATION:
					child.dataType = EBML::DataType::UInt;
					reader.ReadCurrentChildData(child);
					duration = child.data.ui;

					this->demuxerState.leftClusterSize -= child.dataSize;
					break;
				case MATROSKA_ID_REFERENCEBLOCK:
					isKeyframe = false;
					reader.SkipCurrentChildData(child);

					this->demuxerState.leftClusterSize -= child.dataSize;
					break;
				default:
					reader.SkipCurrentChildData(child);

					this->demuxerState.leftClusterSize -= child.dataSize;
				}
			}
			reader.Verify();
			
			if (duration.HasValue())
			{
				for (IncomingPacket& ip : this->demuxerState.packetQueue)
					ip.packet.duration = *duration;
			}
			for (IncomingPacket& ip : this->demuxerState.packetQueue)
				ip.packet.containsKeyframe = isKeyframe;
		}
		break;
		case MATROSKA_ID_SIMPLEBLOCK:
			this->ReadBlockHeader(true, element.dataSize);
			this->demuxerState.leftClusterSize -= element.dataSize;
			break;
			//skip elements that we don't need
		case MATROSKA_ID_POSITION:
		case MATROSKA_ID_PREVSIZE:
			//skip anything that might come after clusters (like indexes and so on)
		case MATROSKA_ID_CUES:
		case MATROSKA_ID_TAGS:
			this->inputStream.Skip(element.dataSize);
			this->demuxerState.leftClusterSize -= element.dataSize;
			break;
		default:
			if (element.dataSize < this->demuxerState.leftClusterSize)
			{
				//we try it
				this->inputStream.Skip(element.dataSize);
				this->demuxerState.leftClusterSize -= element.dataSize;
			}
			else
				this->Resync();
		}
	}

	//get next packet from packetqueue
	IncomingPacket p = this->demuxerState.packetQueue.PopFront();
	if(p.IsBuffered())
		return new Packet(Move(p.packet));

	const TrackInfo& trackInfo = this->tracks[p.trackNumber];

	UniquePointer<Packet> packet = new Packet(p.size + trackInfo.strippedHeader.GetSize());
	if (!trackInfo.strippedHeader.IsEmpty())
		MemCopy(packet->GetData(), &trackInfo.strippedHeader[0], trackInfo.strippedHeader.GetSize());
	this->inputStream.ReadBytes(packet->GetData() + trackInfo.strippedHeader.GetSize(), p.size);
	packet->CopyAttributesFrom(p.packet);

	this->demuxerState.leftClusterSize -= p.size;
	
	return packet;
}

//Private methods
void MatroskaDemuxer::AddStream(Matroska::Track &track)
{
	//create stream
	Stream *pStream;
	switch(track.type)
	{
	case TRACK_TYPE_AUDIO:
		pStream = new Stream(DataType::Audio);
		break;
	case TRACK_TYPE_SUBTITLE:
		pStream = new Stream(DataType::Subtitle);
		break;
	case TRACK_TYPE_VIDEO:
		pStream = new Stream(DataType::Video);
		break;
	default:
		NOT_IMPLEMENTED_ERROR;
		pStream = nullptr;
	}
	
	pStream->timeScale = this->timeScale;
	CodingFormatId codingFormatId = this->codecIdMap.Get(track.codecId);
	pStream->SetCodingFormat(codingFormatId);
	
	uint32 index = Demuxer::AddStream(pStream);
	this->tracks[track.number].streamIndex = index;

	if (!track.codecPrivate.IsEmpty())
		pStream->codingParameters.codecPrivateData = FixedSizeBuffer(&track.codecPrivate[0], track.codecPrivate.GetSize());

	//encoding
	if (!track.encodings.IsEmpty())
	{
		ASSERT(track.encodings.GetNumberOfElements() == 1, u8"TODO: Multiple encodings is currently not implemented.");
		const TrackEncoding& trackEncoding = track.encodings[0];

		ASSERT(trackEncoding.scope == TrackEncodingScope::AllFrameContent, u8"TODO: Only frame data encoding is currently implemented.");
		ASSERT(trackEncoding.type == TrackEncodingType::Compression, u8"TODO: Only compression is currently implemented (not encryption).");

		//header stripping
		ASSERT(trackEncoding.compression.algorithm == TrackCompressionAlgorithm::HeaderStripping, u8"TODO: Only header stripping compression is currently implemented.");
		this->tracks[track.number].strippedHeader = trackEncoding.compression.settings;
	}
	
	//stream specific
	switch(track.type)
	{
	case TRACK_TYPE_AUDIO:
	{
		pStream->codingParameters.audio.sampleRate = (uint32)track.audio.sampleRate;
		//audioStream->sampleFormatHints.nChannels = track.audio.nChannels;

		if (pStream->codingParameters.codingFormat)
		{
			switch (pStream->codingParameters.codingFormat->GetId())
			{
			case CodingFormatId::AAC:
				pStream->parserFlags.requiresParsing = false;
				//ADTS is removed and frames are muxed on frame boundaries
				break;
			}
		}
		
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
		pStream->codingParameters.video.size.width = track.video.width;
		pStream->codingParameters.video.size.height = track.video.height;

		//check for microsoft BMP header
		if (codingFormatId == CodingFormatId::Unknown && track.codecId == codecId_ms_fourcc)
		{
			bool isBottomUp;
			BufferInputStream codecPrivateData(&track.codecPrivate[0], track.codecPrivate.GetSize());
			_stdxx_::ReadBMPHeader(isBottomUp, codecPrivateData, *pStream);
		}

		if (pStream->codingParameters.codingFormat)
		{
			switch (pStream->codingParameters.codingFormat->GetId())
			{
			case CodingFormatId::H264:
				//only look at packets
				pStream->parserFlags.requiresParsing = true;
				pStream->parserFlags.repack = false;
				break;
			}
		}
	}
	break;
	}
}

void MatroskaDemuxer::BufferPackets()
{
	for (IncomingPacket& ip : this->demuxerState.packetQueue)
	{
		ASSERT(!ip.IsBuffered(), u8"??? can't be. Report please!");

		const TrackInfo& trackInfo = this->tracks[ip.trackNumber];

		ip.packet.Allocate(ip.size + trackInfo.strippedHeader.GetSize());
		if (!trackInfo.strippedHeader.IsEmpty())
			MemCopy(ip.packet.GetData(), &trackInfo.strippedHeader[0], trackInfo.strippedHeader.GetSize());
		this->inputStream.ReadBytes(ip.packet.GetData() + trackInfo.strippedHeader.GetSize(), ip.size);
		ip.size = 0; //the packet is now bufferd
	}
}

uint8 MatroskaDemuxer::ReadBlockHeader(bool simple, uint32 blockSize)
{
	DataReader reader(true, this->inputStream);
	
	uint8 length;
	uint64 trackNumber = EBML::DecodeVariableLengthInteger(length, this->inputStream);
	int16 timeCode = reader.ReadInt16();
	uint8 flags = reader.ReadByte();
	uint8 blockHeaderSize = length + 2 + 1;
	
	Packet attribs;
	attribs.streamIndex = this->tracks[trackNumber].streamIndex;
	attribs.pts = this->demuxerState.clusterTimecode + timeCode;
	attribs.containsKeyframe = simple && ((flags & 0x80) != 0);

	switch ((flags >> 1) & 3)
	{
	case 0: //no lacing
	{
		IncomingPacket nextPacket;
		nextPacket.trackNumber = trackNumber;
		nextPacket.size = blockSize - blockHeaderSize;
		nextPacket.packet.CopyAttributesFrom(attribs);
		this->demuxerState.packetQueue.InsertTail(nextPacket);
	}
	break;
	case 1: //Xiph lacing
	{
		IncomingPacket nextPacket;
		nextPacket.trackNumber = trackNumber;
		nextPacket.packet.CopyAttributesFrom(attribs);

		uint8 nEncodedLaceSizes = reader.ReadByte();
		blockHeaderSize++;
		uint32 sum = 0;
		for (uint8 i = 0; i < nEncodedLaceSizes; i++)
		{
			uint32 frameSize = 0;
			while (true)
			{
				uint8 delta = reader.ReadByte();
				blockHeaderSize++;
				frameSize += delta;
				if (delta != Unsigned<uint8>::Max())
					break;
			}
			sum += frameSize;
			nextPacket.size = frameSize;
			this->demuxerState.packetQueue.InsertTail(nextPacket);
		}

		nextPacket.size = blockSize - blockHeaderSize - sum;
		this->demuxerState.packetQueue.InsertTail(nextPacket);
	}
	break;
	case 2: //fixed-size lacing
	{
		uint8 nFrames = reader.ReadByte() + 1;
		blockHeaderSize++;

		IncomingPacket nextPacket;
		nextPacket.trackNumber = trackNumber;
		nextPacket.packet.CopyAttributesFrom(attribs);

		uint32 frameSize = (blockSize - blockHeaderSize) / nFrames;
		for (uint8 i = 0; i < nFrames; i++)
		{
			nextPacket.size = frameSize;
			this->demuxerState.packetQueue.InsertTail(nextPacket);
		}
	}
	break;
	case 3: //EBML lacing
	{
		uint8 nEncodedLaceSizes = reader.ReadByte();
		blockHeaderSize++;
		uint64 laceSize = EBML::DecodeVariableLengthInteger(length, this->inputStream);
		blockHeaderSize += length;

		IncomingPacket nextPacket;
		nextPacket.trackNumber = trackNumber;
		nextPacket.size = laceSize;
		nextPacket.packet.CopyAttributesFrom(attribs);
		this->demuxerState.packetQueue.InsertTail(nextPacket);
		nEncodedLaceSizes--;

		uint64 sum = laceSize;
		for (uint8 i = 0; i < nEncodedLaceSizes; i++)
		{
			uint64 codedSize = EBML::DecodeVariableLengthInteger(length, this->inputStream);
			blockHeaderSize += length;
			int64 rangeShift = (1 << (length * 8 - length - 1)) - 1;
			laceSize += int64(codedSize) - rangeShift;

			nextPacket.size = laceSize;
			this->demuxerState.packetQueue.InsertTail(nextPacket);

			sum += laceSize;
		}

		nextPacket.size = blockSize - blockHeaderSize - sum;
		this->demuxerState.packetQueue.InsertTail(nextPacket);
	}
	break;
	default:
		NOT_IMPLEMENTED_ERROR;
	}

	return blockHeaderSize;
}

void MatroskaDemuxer::ReadSegment(uint64 segmentOffset, bool isLive)
{
	BinaryTreeMap<uint64, uint64> idOffsetMap;
	BinaryTreeSet<uint64> readSections;
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
			this->demuxerState.leftClusterSize = element.dataSize;
			foundCluster = true;
			break; //we are at the clusters.... break from here
		case EBML::ClassId::EBML_ID_VOID:
		case MATROSKA_ID_TAGS:
			//just skip these
			readSections.Insert(element.id);
			this->inputStream.Skip(element.dataSize);
			break;
		default: //unknown... skip
			readSections.Insert(element.id);
			if (isLive) //could any time be any weird data that is not ebml. unfortunately this is allowed when ebml is used for live streaming-.-
				this->Resync();
			else
				this->inputStream.Skip(element.dataSize);
		}
	}

	for (const auto &kv : idOffsetMap)
	{
		if (!readSections.Contains(kv.key))
		{
			uint64 currentOffset = this->inputStream.QueryCurrentOffset();

			this->inputStream.SeekTo(kv.value);
			EBML::Element element;
			EBML::ParseElementHeader(element, this->inputStream);
			this->ReadSection(element);

			this->inputStream.SeekTo(currentOffset);
		}
	}

	ASSERT(foundCluster, u8"if you see this, report this please.")
}

void MatroskaDemuxer::ReadSection(const EBML::Element &element)
{
	switch (element.id)
	{
	case MATROSKA_ID_CLUSTER:
		this->demuxerState.leftClusterSize = element.dataSize;
		//dont read it
		break;
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
		{
			this->AddStream(track);
		}
	}
	break;
	default:
		this->inputStream.Skip(element.dataSize);
	}
}

void MatroskaDemuxer::Reset()
{
	Demuxer::Reset();

	this->demuxerState.packetQueue.Release();
}

void MatroskaDemuxer::Resync()
{
	DataReader reader(true, this->inputStream);

	uint32 id = reader.ReadUInt32();
	bool found = false;
	while (!found)
	{
		switch (id)
		{
		case MATROSKA_ID_INFO:
		case MATROSKA_ID_CLUSTER:
			found = true;
			break;
		default:
			id = (id << 8) | reader.ReadByte();
		}
	}

	//found a valid id
	this->inputStream.Rewind(4);
}
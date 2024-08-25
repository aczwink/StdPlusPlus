/*
 * Copyright (c) 2017-2024 Amir Czwink (amir130@hotmail.de)
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
#include "MatroskaMuxer.hpp"
//Local
#include <Std++/Signed.hpp>
#include <Std++/Streams/Writers/DataWriter.hpp>
#include <Std++/Multimedia/CodingParameters.hpp>
#include "../BMP/BMP.hpp"
#include "EBML.hpp"

//Constructor
MatroskaMuxer::MatroskaMuxer(const ContainerFormat &refFormat, SeekableOutputStream &refOutput) : Muxer(refFormat, refOutput)
{
	this->currentCluster.isClusterOpen = false;
	this->currentCluster.basePTS = 0;
	this->currentCluster.pts = 0;
}

//Private methods
void MatroskaMuxer::BeginElement(MatroskaId id)
{
	this->WriteId(id);
	this->elementSizeOffsets.Push(this->outputStream.QueryCurrentOffset());

	DataWriter dataWriter(true, this->outputStream);
	dataWriter.WriteUInt64(0); //we write size always as 64bit value because we don't know the size upfront
}

void MatroskaMuxer::ComputeTimeScales()
{
	this->UpdateTimingInfo();

	ASSERT(this->timeScale.numerator == 1, "If you see this, report to StdXX"); //TODO: what happens if not?

	//make sure time scale is precise enough
	while(this->MapSeconds(30) > Signed<int16>::Max())
	{
		this->timeScale.denominator /= 2;
		this->duration /= 2;
	}

	this->timeCodeScale = this->timeScale * (uint64)1000000000; //timecode scale is in nano seconds
}

void MatroskaMuxer::EndElement()
{
	uint64 currentOffset, sizeOffset;

	currentOffset = this->outputStream.QueryCurrentOffset();
	sizeOffset = this->elementSizeOffsets.Pop();

	//write size
	this->outputStream.SeekTo(sizeOffset);
	DataWriter dataWriter(true, this->outputStream);
	dataWriter.WriteUInt64(((uint64)1 << 56) | (currentOffset - sizeOffset - 8)); //8 is the 'size' field size

	//
	this->outputStream.SeekTo(currentOffset);
}

void MatroskaMuxer::FinalizeMetaSeekInfo()
{
	DataWriter dataWriter(true, this->outputStream);

	//segment info
	this->outputStream.SeekTo(this->metaSeekInfoWriteOffsets.segmentInfoOffset);
	dataWriter.WriteUInt64(this->metaSeekInfoOffsets.segmentInfoOffset - this->segmentOutputStreamOffset);

	//track info
	this->outputStream.SeekTo(this->metaSeekInfoWriteOffsets.trackInfoOffset);
	dataWriter.WriteUInt64(this->metaSeekInfoOffsets.trackInfoOffset - this->segmentOutputStreamOffset);

	//cueing data
	this->outputStream.SeekTo(this->metaSeekInfoWriteOffsets.cueingDataOffset);
	dataWriter.WriteUInt64(this->metaSeekInfoOffsets.cueingDataOffset - this->segmentOutputStreamOffset);
}

uint64 MatroskaMuxer::PrepareMetaSeekEntry(MatroskaId id)
{
	DataWriter dataWriter(true, this->outputStream);

	uint64 offset;

	this->BeginElement(MATROSKA_ID_SEEK);

	//write SeekID
	this->BeginElement(MATROSKA_ID_SEEKID);
	this->WriteId(id);
	this->EndElement();

	//keep enough space for SeekPosition
	this->BeginElement(MATROSKA_ID_SEEKPOSITION);
	offset = this->outputStream.QueryCurrentOffset();
	dataWriter.WriteUInt64(0);
	this->EndElement();

	//end Seek
	this->EndElement();

	return offset;
}

void MatroskaMuxer::PrepareMetaSeekInfo()
{
	this->BeginElement(MATROSKA_ID_SEEKHEAD);

	this->metaSeekInfoWriteOffsets.segmentInfoOffset = this->PrepareMetaSeekEntry(MATROSKA_ID_INFO);
	this->metaSeekInfoWriteOffsets.trackInfoOffset = this->PrepareMetaSeekEntry(MATROSKA_ID_TRACKS);
	this->metaSeekInfoWriteOffsets.cueingDataOffset = this->PrepareMetaSeekEntry(MATROSKA_ID_CUES);

	this->EndElement();
}

void MatroskaMuxer::WriteEBMLUInt(uint64 value)
{
	if(value < 126) //2^7 - 2
	{
		byte b = 0x80 | (byte)value;
		this->outputStream.WriteBytes(&b, 1);
	}
	else
	{
		NOT_IMPLEMENTED_ERROR;
	}
}

void MatroskaMuxer::WriteAdditionalAudioStreamInfo(Stream &refStream)
{
	switch(refStream.codingParameters.codingFormat->GetId())
	{
		case CodingFormatId::PCM_Float32LE:
			this->WriteUIntElement(MATROSKA_ID_BITDEPTH, 32);
			break;
	}
}

void MatroskaMuxer::WriteCodecElement(Stream &stream)
{
	ASSERT(stream.codingParameters.codingFormat, u8"If you see this, report to StdXX");

	switch(stream.codingParameters.dataType)
	{
		case DataType::Audio:
		{
			switch(stream.codingParameters.codingFormat->GetId())
			{
				case CodingFormatId::PCM_Float32LE:
					this->WriteASCIIElement(MATROSKA_ID_CODECID, MATROSKA_CODEC_PCM_FLOAT_LE);
					break;
				default:
					NOT_IMPLEMENTED_ERROR; //TODO: next
			}
		}
		break;
		case DataType::Subtitle:
			NOT_IMPLEMENTED_ERROR; //TODO: next
			break;
		case DataType::Video:
			{
				//native codecs
				switch(stream.codingParameters.codingFormat->GetId())
				{
					default: //non-native
						this->WriteASCIIElement(MATROSKA_ID_CODECID, codecId_ms_fourcc);
						this->BeginElement(MATROSKA_ID_CODECPRIVATE);
						_stdxx_::WriteBitmapInfoHeader(stream, this->outputStream);
						this->EndElement();
				}
			}
			break;
	}
	/*

	String codecId = MapToCodecString(stream.GetCodec()->GetId());
	if(!codecId.IsEmpty())
	{
		this->WriteASCIIElement(MATROSKA_ID_CODECID, codecId);

		return;
	}

	switch(stream.GetCodec()->Get())
	{
		case CodecId::PCM_S16LE:
		{
			this->WriteASCIIElement(MATROSKA_ID_CODECID, CODEC_PCM_INTEGER_LE);
		}
			break;
		default:
			NOT_IMPLEMENTED_ERROR;
	}*/
}

void MatroskaMuxer::WriteCuePoints()
{
	for(const auto &kv : this->cues)
	{
		this->BeginElement(MATROSKA_ID_CUEPOINT);

		this->WriteUIntElement(MATROSKA_ID_CUETIME, kv.key);

		for(uint32 streamIndex : kv.value.streamIndices)
		{
			this->BeginElement(MATROSKA_ID_CUETRACKPOSITIONS);

			this->WriteUIntElement(MATROSKA_ID_CUETRACK, streamIndex + 1);
			this->WriteUIntElement(MATROSKA_ID_CUECLUSTERPOSITION, kv.value.clusterOffset - this->segmentOutputStreamOffset);
			this->WriteUIntElement(MATROSKA_ID_CUERELATIVEPOSITION, kv.value.relativeOffset);

			this->EndElement();
		}

		this->EndElement();
	}
}

void MatroskaMuxer::WriteId(MatroskaId id)
{
	DataWriter writer(true, this->outputStream);

	if((id & 0xFF) == id)
	{
		//Class A
		writer.WriteByte(id & 0xFF);
	}
	else if((id & 0xFFFF) == id)
	{
		//Class B
		writer.WriteUInt16(id & 0xFFFF);
	}
	else if((id & 0xFFFFFF) == id)
	{
		//Class C
		writer.WriteByte((id >> 16) & 0xFF);
		writer.WriteUInt16(id & 0xFFFF);
	}
	else
	{
		//Class D
		writer.WriteUInt32(id);
	}
}

void MatroskaMuxer::WriteUInt(uint64 value)
{
	DataWriter writer(true, this->outputStream);

	if(value == 0)
	{
		writer.WriteByte(0);
		return;
	}

	//trim higher zero bytes
	for(uint8 i = 8; i > 0; i--)
	{
		if(value & ((uint64)0xFF << 56))
		{
			//write remaining bytes
			while(i--)
			{
				writer.WriteByte(static_cast<byte>(value >> 56u));
				value <<= 8u;
			}
			return;
		}

		value <<= 8;
	}
}

//Public methods
void MatroskaMuxer::Finalize()
{
	//end laster cluster if needed
	if(this->currentCluster.isClusterOpen)
		this->EndElement();

	//Cueing data
	this->metaSeekInfoOffsets.cueingDataOffset = this->outputStream.QueryCurrentOffset();
	this->BeginElement(MATROSKA_ID_CUES);
	this->WriteCuePoints();
	this->EndElement();

	//end segment
	this->EndElement();

	//finalize other elements
	this->FinalizeMetaSeekInfo();
}

void MatroskaMuxer::WriteHeader()
{
	uint32 i;

	//do precomputations
	this->ComputeTimeScales();

	//write the EBML header
	this->BeginElement((MatroskaId)EBML::EBML_ID_EBML);

	this->WriteUIntElement(MATROSKA_ID_EBMLVERSION, 1);
	this->WriteUIntElement(MATROSKA_ID_EBMLREADVERSION, 1);
	this->WriteUIntElement(MATROSKA_ID_EBMLMAXIDLENGTH, 4);
	this->WriteUIntElement(MATROSKA_ID_EBMLMAXSIZELENGTH, 8);
	this->WriteASCIIElement((MatroskaId)EBML::EBML_ID_DOCTYPE, u8"matroska");
	this->WriteUIntElement((MatroskaId)EBML::EBML_ID_DOCTYPEVERSION, 2); //in order to use "SimpleBlock" it must be at least 2
	this->WriteUIntElement((MatroskaId)EBML::EBML_ID_DOCTYPEREADVERSION, 2);

	this->EndElement();

	//begin with segment
	this->BeginElement(MATROSKA_ID_SEGMENT);
	this->segmentOutputStreamOffset = this->outputStream.QueryCurrentOffset();

	//Meta Seek Info
	this->PrepareMetaSeekInfo();

	//segment information
	this->metaSeekInfoOffsets.segmentInfoOffset = this->outputStream.QueryCurrentOffset();
	this->BeginElement(MATROSKA_ID_INFO);

	this->WriteUIntElement(MATROSKA_ID_TIMECODESCALE, this->timeCodeScale.ToFloat());
	this->WriteFloatElement(MATROSKA_ID_DURATION, this->duration);
	this->WriteUTF8Element(MATROSKA_ID_MUXINGAPP, u8"Std++ library");
	this->WriteUTF8Element(MATROSKA_ID_WRITINGAPP, u8"Std++ library");

	this->EndElement();

	//tracks
	this->metaSeekInfoOffsets.trackInfoOffset = this->outputStream.QueryCurrentOffset();
	this->BeginElement(MATROSKA_ID_TRACKS);
	for(i = 0; i < this->GetNumberOfStreams(); i++)
	{
		Stream *const& stream = this->GetStream(i);

		this->BeginElement(MATROSKA_ID_TRACKENTRY);

		//TrackNumber
		this->WriteUIntElement(MATROSKA_ID_TRACKNUMBER, i + 1);

		//TrackUID
		this->WriteUIntElement(MATROSKA_ID_TRACKUID, i + 1);

		//TrackType
		switch(stream->codingParameters.dataType)
		{
			case DataType::Audio:
				this->WriteUIntElement(MATROSKA_ID_TRACKTYPE, TRACK_TYPE_AUDIO);
				break;
			case DataType::Subtitle:
				this->WriteUIntElement(MATROSKA_ID_TRACKTYPE, TRACK_TYPE_SUBTITLE);
				break;
			case DataType::Video:
				this->WriteUIntElement(MATROSKA_ID_TRACKTYPE, TRACK_TYPE_VIDEO);
				break;
		}

		//DefaultDuration
		this->WriteUIntElement(MATROSKA_ID_DEFAULTDURATION,
							   static_cast<uint64>((stream->timeScale.Reciprocal() * uint64(1000000000)).ToFloat()));

		//TODO: Language

		//CodecID
		this->WriteCodecElement(*stream);

		switch(stream->codingParameters.dataType)
		{
			case DataType::Audio:
			{
				this->BeginElement(MATROSKA_ID_AUDIO);

				this->WriteUIntElement(MATROSKA_ID_CHANNELS, stream->codingParameters.audio.sampleFormat->nChannels);
				this->WriteFloatElement(MATROSKA_ID_SAMPLINGFREQUENCY, stream->codingParameters.audio.sampleRate);

				this->WriteAdditionalAudioStreamInfo(*stream);

				this->EndElement();
			}
				break;
			case DataType::Video:
			{
				this->BeginElement(MATROSKA_ID_VIDEO);

				this->WriteUIntElement(MATROSKA_ID_PIXELWIDTH, stream->codingParameters.video.size.width);
				this->WriteUIntElement(MATROSKA_ID_PIXELHEIGHT, stream->codingParameters.video.size.height);

				this->EndElement();
			}
				break;
		}
		this->EndElement();
	}
	this->EndElement();

	//TODO: chapters
}

void MatroskaMuxer::WritePacket(const IPacket& packet)
{
	uint64 transformedPTS = this->TransformPTS(packet.GetPresentationTimestamp(), packet.GetStreamIndex());

	//check for new cluster
	bool startNewCluster = false;

	if(!this->currentCluster.isClusterOpen)
	{
		//if we don't have a cluster open we need to open one
		startNewCluster = true;
	}
	else if(this->GetStream(packet.GetStreamIndex())->codingParameters.dataType == DataType::Video)
	{
		/*
		As of here https://www.matroska.org/technical/diagram/index.html
		it seems that a new cluster should be created every 5 MB or every 5 seconds - whichever comes first.
		*/
		if(this->currentCluster.size > 5 * MiB)
			startNewCluster = true;
		if(this->currentCluster.pts > this->currentCluster.basePTS + this->MapSeconds(5))
			startNewCluster = true;
	}

	if(startNewCluster)
	{
		if(this->currentCluster.isClusterOpen)
			this->EndElement(); //end last cluster

		//Cluster
		this->currentCluster.beginOffset = this->outputStream.QueryCurrentOffset();
		this->BeginElement(MATROSKA_ID_CLUSTER);
		this->currentCluster.isClusterOpen = true;
		this->currentCluster.size = 0;
		if(transformedPTS != Unsigned<uint64>::Max())
			this->currentCluster.basePTS = transformedPTS;
		else
			this->currentCluster.basePTS = this->currentCluster.pts;
		this->currentCluster.pts = this->currentCluster.basePTS;

		this->WriteUIntElement(MATROSKA_ID_TIMECODE, this->currentCluster.basePTS);
	}

	//write block header
	this->BeginElement(MATROSKA_ID_SIMPLEBLOCK);
	this->WriteEBMLUInt(packet.GetStreamIndex() + 1);

	/*
	add cue

	As of here https://www.matroska.org/technical/diagram/index.html
	it is sufficient to index video keyframes
	*/
	if(packet.ContainsKeyFrame() && transformedPTS != Unsigned<uint64>::Max() && this->GetStream(packet.GetStreamIndex())->codingParameters.dataType == DataType::Video)
	{
		CueEntry &cueEntry = this->cues[transformedPTS];

		cueEntry.clusterOffset = this->currentCluster.beginOffset;
		cueEntry.relativeOffset = static_cast<uint32>(this->outputStream.QueryCurrentOffset() - cueEntry.clusterOffset);
		cueEntry.streamIndices.Push(packet.GetStreamIndex());
	}

	//update current pts
	if(transformedPTS != Unsigned<uint64>::Max() && transformedPTS > this->currentCluster.pts)
		this->currentCluster.pts = transformedPTS;

	//write packet header
	DataWriter writer(true, this->outputStream);

	int16 pts = int16(transformedPTS - this->currentCluster.basePTS);
	writer.WriteInt16(pts);

	uint8 flags = 0;
	if(packet.ContainsKeyFrame())
	{
		flags |= 0x80;
	}
	writer.WriteByte(flags);

	//write payload
	this->outputStream.WriteBytes(packet.GetData(), packet.GetSize());
	this->EndElement();

	this->currentCluster.size += packet.GetSize();
}
//Class header
#include "CMatroskaMuxer.h"
//

//Constructor
CMatroskaMuxer::CMatroskaMuxer(const IFormat &refFormat, ASeekableOutputStream &refOutput) : AMuxer(refFormat, refOutput)
{
	this->currentCluster.isClusterOpen = false;
	this->currentCluster.basePTS = 0;
	this->currentCluster.pts = 0;
}

//Private methods
void CMatroskaMuxer::BeginElement(EMatroskaId id)
{
	this->WriteId(id);
	this->elementSizeOffsets.Push(this->refOutput.GetCurrentOffset());
	this->refOutput.WriteUInt64BE(0); //we write size always as 64bit value because we don't know the size upfront
}

void CMatroskaMuxer::ComputeTimeScales()
{
	this->UpdateTimingInfo();

	ASSERT(this->timeScale.numerator == 1); //TODO: what happens if not?
											
	//make sure time scale is precise enough
	while(this->MapSeconds(30) > INT16_MAX)
	{
		this->timeScale.denominator /= 2;
		this->duration /= 2;
	}

	this->timeCodeScale = this->timeScale * (uint64)1000000000; //timecode scale is in nano seconds
}

void CMatroskaMuxer::EndElement()
{
	uint64 currentOffset, sizeOffset;

	currentOffset = this->refOutput.GetCurrentOffset();
	sizeOffset = this->elementSizeOffsets.Pop();

	//write size
	this->refOutput.SetCurrentOffset(sizeOffset);
	this->refOutput.WriteUInt64BE(((uint64)1 << 56) | (currentOffset - sizeOffset - 8)); //8 is the 'size' field size

	//
	this->refOutput.SetCurrentOffset(currentOffset);
}

void CMatroskaMuxer::FinalizeMetaSeekInfo()
{
	//segment info
	this->refOutput.SetCurrentOffset(this->metaSeekInfoWriteOffsets.segmentInfoOffset);
	this->refOutput.WriteUInt64BE(this->metaSeekInfoOffsets.segmentInfoOffset - this->segmentOutputStreamOffset);

	//track info
	this->refOutput.SetCurrentOffset(this->metaSeekInfoWriteOffsets.trackInfoOffset);
	this->refOutput.WriteUInt64BE(this->metaSeekInfoOffsets.trackInfoOffset - this->segmentOutputStreamOffset);

	//cueing data
	this->refOutput.SetCurrentOffset(this->metaSeekInfoWriteOffsets.cueingDataOffset);
	this->refOutput.WriteUInt64BE(this->metaSeekInfoOffsets.cueingDataOffset - this->segmentOutputStreamOffset);
}

uint64 CMatroskaMuxer::PrepareMetaSeekEntry(EMatroskaId id)
{
	uint64 offset;
	
	this->BeginElement(MATROSKA_ID_SEEK);

	//write SeekID
	this->BeginElement(MATROSKA_ID_SEEKID);
	this->WriteId(id);
	this->EndElement();

	//keep enough space for SeekPosition
	this->BeginElement(MATROSKA_ID_SEEKPOSITION);
	offset = this->refOutput.GetCurrentOffset();
	this->refOutput.WriteUInt64BE(0);
	this->EndElement();

	//end Seek
	this->EndElement();

	return offset;
}

void CMatroskaMuxer::PrepareMetaSeekInfo()
{
	this->BeginElement(MATROSKA_ID_SEEKHEAD);
	
	this->metaSeekInfoWriteOffsets.segmentInfoOffset = this->PrepareMetaSeekEntry(MATROSKA_ID_INFO);
	this->metaSeekInfoWriteOffsets.trackInfoOffset = this->PrepareMetaSeekEntry(MATROSKA_ID_TRACKS);
	this->metaSeekInfoWriteOffsets.cueingDataOffset = this->PrepareMetaSeekEntry(MATROSKA_ID_CUES);
	
	this->EndElement();
}

void CMatroskaMuxer::WriteEBMLUInt(uint64 value)
{
	if(value < 126) //2^7 - 2
	{
		this->refOutput.WriteByte(0x80 | (byte)value);
	}
	else
	{
		ASSERT(0);
	}
}

void CMatroskaMuxer::WriteAdditionalAudioStreamInfo(CAudioStream &refStream)
{
	ASSERT(refStream.GetCodec());
	
	switch(refStream.GetCodec()->GetId())
	{
	case ECodecId::PCM_Float32LE:
		{
			this->WriteUIntElement(MATROSKA_ID_BITDEPTH, 32);
		}
		break;
	}
}

void CMatroskaMuxer::WriteCodecElement(AStream &refStream)
{
	C8BitString codecId;
	
	ASSERT(refStream.GetCodec());

	codecId = MapToCodecString(refStream.GetCodec()->GetId());
	if(!codecId.IsEmpty())
	{
		this->WriteASCIIElement(MATROSKA_ID_CODECID, codecId);
		
		return;
	}

	switch(refStream.GetCodec()->GetId())
	{
	case ECodecId::PCM_Float32LE:
		{
			this->WriteASCIIElement(MATROSKA_ID_CODECID, CODEC_PCM_FLOAT_LE);
		}
		break;
	case ECodecId::PCM_S16LE:
		{
			this->WriteASCIIElement(MATROSKA_ID_CODECID, CODEC_PCM_INTEGER_LE);
		}
		break;
	default:
		NOT_IMPLEMENTED_ERROR;
	}
}

void CMatroskaMuxer::WriteCuePoints()
{
	for(const auto &refKV : this->cues)
	{
		this->BeginElement(MATROSKA_ID_CUEPOINT);

		this->WriteUIntElement(MATROSKA_ID_CUETIME, refKV.key);

		for(uint32 streamIndex : refKV.value.streamIndices)
		{
			this->BeginElement(MATROSKA_ID_CUETRACKPOSITIONS);

			this->WriteUIntElement(MATROSKA_ID_CUETRACK, streamIndex + 1);
			this->WriteUIntElement(MATROSKA_ID_CUECLUSTERPOSITION, refKV.value.clusterOffset - this->segmentOutputStreamOffset);

			this->EndElement();
		}

		this->EndElement();
	}
}

void CMatroskaMuxer::WriteId(EMatroskaId id)
{
	if((id & 0xFF) == id)
	{
		//Class A
		this->refOutput.WriteByte(id & 0xFF);
	}
	else if((id & 0xFFFF) == id)
	{
		//Class B
		this->refOutput.WriteUInt16BE(id & 0xFFFF);
	}
	else if((id & 0xFFFFFF) == id)
	{
		//Class C
		this->refOutput.WriteByte((id >> 16) & 0xFF);
		this->refOutput.WriteUInt16BE(id & 0xFFFF);
	}
	else
	{
		//Class D
		this->refOutput.WriteUInt32BE(id);
	}
}

void CMatroskaMuxer::WriteUInt(uint64 value)
{
	if(value == 0)
	{
		this->refOutput.WriteByte(0);
		return;
	}
	
	//trim higher zero bytes
	for(uint8 i = 8; i > 0; i--)
	{
		if(value & ((uint64)0xFF << 56))
		{
			//write remaining bytes
			while(value & ((uint64)0xFF << 56))
			{
				this->refOutput.WriteByte(value >> 56);
				value <<= 8;
			}
			return;
		}

		value <<= 8;
	}
}

//Public methods
void CMatroskaMuxer::Finalize()
{
	//end laster cluster if needed
	if(this->currentCluster.isClusterOpen)
		this->EndElement();

	//Cueing data
	this->metaSeekInfoOffsets.cueingDataOffset = this->refOutput.GetCurrentOffset();
	this->BeginElement(MATROSKA_ID_CUES);
	this->WriteCuePoints();
	this->EndElement();
	
	//end segment
	this->EndElement();

	//finalize other elements
	this->FinalizeMetaSeekInfo();
}

void CMatroskaMuxer::WriteHeader()
{
	uint32 i;

	//do precomputations
	this->ComputeTimeScales();
	
	//write the EBML header
	this->BeginElement(MATROSKA_ID_EBML);
	
	this->WriteUIntElement(MATROSKA_ID_EBMLVERSION, 1);
	this->WriteUIntElement(MATROSKA_ID_EBMLREADVERSION, 1);
	this->WriteUIntElement(MATROSKA_ID_EBMLMAXIDLENGTH, 4);
	this->WriteUIntElement(MATROSKA_ID_EBMLMAXSIZELENGTH, 8);
	this->WriteASCIIElement(MATROSKA_ID_DOCTYPE, "matroska");
	this->WriteUIntElement(MATROSKA_ID_DOCTYPEVERSION, 2); //in order to use "SimpleBlock" it must be at least 2
	this->WriteUIntElement(MATROSKA_ID_DOCTYPEREADVERSION, 2);
	
	this->EndElement();

	//begin with segment
	this->BeginElement(MATROSKA_ID_SEGMENT);
	this->segmentOutputStreamOffset = this->refOutput.GetCurrentOffset();

	//Meta Seek Info
	this->PrepareMetaSeekInfo();
	
	//segment information
	this->metaSeekInfoOffsets.segmentInfoOffset = this->refOutput.GetCurrentOffset();
	this->BeginElement(MATROSKA_ID_INFO);
	
	this->WriteUIntElement(MATROSKA_ID_TIMECODESCALE, this->timeCodeScale.Eval());
	this->WriteFloatElement(MATROSKA_ID_DURATION, this->duration);
	this->WriteUTF8Element(MATROSKA_ID_MUXINGAPP, "ACMultimedia Library");
	this->WriteUTF8Element(MATROSKA_ID_WRITINGAPP, "ACMultimedia Library");

	this->EndElement();

	//tracks
	this->metaSeekInfoOffsets.trackInfoOffset = this->refOutput.GetCurrentOffset();
	this->BeginElement(MATROSKA_ID_TRACKS);
	for(i = 0; i < this->GetNumberOfStreams(); i++)
	{
		AStream *const& refpStream = this->GetStream(i);
		
		this->BeginElement(MATROSKA_ID_TRACKENTRY);

		//TrackNumber
		this->WriteUIntElement(MATROSKA_ID_TRACKNUMBER, i + 1);
		
		//TrackUID
		this->WriteUIntElement(MATROSKA_ID_TRACKUID, i + 1);
		
		//TrackType
		switch(refpStream->GetType())
		{
		case EDataType::Audio:
			this->WriteUIntElement(MATROSKA_ID_TRACKTYPE, TRACK_TYPE_AUDIO);
			break;
		case EDataType::Subtitle:
			this->WriteUIntElement(MATROSKA_ID_TRACKTYPE, TRACK_TYPE_SUBTITLE);
			break;
		case EDataType::Video:
			this->WriteUIntElement(MATROSKA_ID_TRACKTYPE, TRACK_TYPE_VIDEO);
			break;
		}

		//TODO: Language
		
		//CodecID
		this->WriteCodecElement(*refpStream);
		
		switch(refpStream->GetType())
		{
		case EDataType::Audio:
			{
				CAudioStream *const& refpAudioStream = (CAudioStream *)refpStream;
				
				this->BeginElement(MATROSKA_ID_AUDIO);
				
				this->WriteUIntElement(MATROSKA_ID_CHANNELS, refpAudioStream->nChannels);
				this->WriteFloatElement(MATROSKA_ID_SAMPLINGFREQUENCY, refpAudioStream->sampleRate);
				
				this->WriteAdditionalAudioStreamInfo(*refpAudioStream);
				
				this->EndElement();
			}
			break;
		case EDataType::Video:
			{
				CVideoStream *const& refpVideoStream = (CVideoStream *)refpStream;

				this->BeginElement(MATROSKA_ID_VIDEO);

				this->WriteUIntElement(MATROSKA_ID_PIXELWIDTH, refpVideoStream->width);
				this->WriteUIntElement(MATROSKA_ID_PIXELHEIGHT, refpVideoStream->height);
				
				this->EndElement();
			}
			break;
		}
		this->EndElement();
	}
	this->EndElement();
	
	//TODO: chapters
}

void CMatroskaMuxer::WritePacket(const SPacket &refPacket)
{
	bool startNewCluster;
	uint8 flags;
	int16 pts;
	uint64 transformedPTS;
	
	transformedPTS = this->TransformPTS(refPacket.pts, refPacket.streamIndex);
	
	startNewCluster = false;
	if(!this->currentCluster.isClusterOpen)
	{
		//if we don't have a cluster open we need to open one
		startNewCluster = true;
	}
	else if(refPacket.containsKeyframe && this->GetStream(refPacket.streamIndex)->GetType() == EDataType::Video)
	{
		/*
		As of here https://www.matroska.org/technical/diagram/index.html
		it seems that a new cluster should be created every 5 MB, every 5 seconds.
		We should only create them when we encounter keyframes of (theoretically all) video streams.
		This is necessary for efficient seeking because previous clusters than don't have to be decoded because of P- or B-frames.
		*/
		if(this->currentCluster.size > 5 * MiB)
			startNewCluster = true;
		if(this->currentCluster.pts > this->currentCluster.basePTS + 5000)
			startNewCluster = true;
	}
	
	if(startNewCluster)
	{
		if(this->currentCluster.isClusterOpen)
			this->EndElement(); //end last cluster
								
		//Cluster
		this->currentCluster.beginOffset = this->refOutput.GetCurrentOffset();
		this->BeginElement(MATROSKA_ID_CLUSTER);
		this->currentCluster.isClusterOpen = true;
		this->currentCluster.size = 0;
		if(transformedPTS != UINT64_MAX)
			this->currentCluster.basePTS = transformedPTS;
		else
			this->currentCluster.basePTS = this->currentCluster.pts;
		this->currentCluster.pts = this->currentCluster.basePTS;
		
		this->WriteUIntElement(MATROSKA_ID_TIMECODE, this->currentCluster.basePTS);
	}
	
	/*
	add cue

	As of here https://www.matroska.org/technical/diagram/index.html
	it is sufficient to index video keyframes
	*/
	if(refPacket.containsKeyframe && transformedPTS != UINT64_MAX && this->GetStream(refPacket.streamIndex)->GetType() == EDataType::Video)
	{
		SCueEntry &refCueEntry = this->cues[transformedPTS];

		refCueEntry.clusterOffset = this->currentCluster.beginOffset;
		refCueEntry.streamIndices.Push(refPacket.streamIndex);
	}
	
	//add packet
	if(transformedPTS != UINT64_MAX && transformedPTS > this->currentCluster.pts)
		this->currentCluster.pts = transformedPTS;
	
	pts = int16(transformedPTS - this->currentCluster.basePTS);
	
	flags = 0;
	if(refPacket.containsKeyframe)
	{
		flags |= 0x80;
	}
	
	this->BeginElement(MATROSKA_ID_SIMPLEBLOCK);
	this->WriteEBMLUInt(refPacket.streamIndex + 1);
	this->refOutput.WriteInt16BE(pts);
	this->refOutput.WriteByte(flags);
	this->refOutput.WriteBytes(refPacket.pData, refPacket.size);
	this->EndElement();

	this->currentCluster.size += refPacket.size;
}
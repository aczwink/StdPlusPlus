//Class header
#include "CMatroskaDemuxer.h"
//Local
#include "Matroska.h"
#include "../BMP/BMP.h"
//Namespaces
using namespace Matroska;

//Constructor
CMatroskaDemuxer::CMatroskaDemuxer(const IFormat &refFormat, ASeekableInputStream &refInput) : ADemuxer(refFormat, refInput)
{
	this->timeScale = CFraction(1000000, 1000000000); //default timeCodeScale is 1.000.000 but all units are in nanoseconds
}

//Private methods
void CMatroskaDemuxer::AddStream()
{
	uint32 index;
	AStream *pStream;

	//create stream
	switch(this->parserState.currentTrack.type)
	{
	case TRACK_TYPE_AUDIO:
		pStream = new CAudioStream;
		break;
	case TRACK_TYPE_SUBTITLE:
		pStream = new CSubtitleStream;
		break;
	case TRACK_TYPE_VIDEO:
		pStream = new CVideoStream;
		break;
	default:
		NOT_IMPLEMENTED_ERROR;
		pStream = nullptr;
	}
	
	pStream->timeScale = this->timeScale;
	pStream->SetCodec(this->parserState.currentTrack.codecId);

	index = ADemuxer::AddStream(pStream);
	this->trackToStreamMap[this->parserState.currentTrack.number] = index;

	//stream specific
	switch(this->parserState.currentTrack.type)
	{
	case TRACK_TYPE_AUDIO:
		{
			CAudioStream *const& refpAudioStream = (CAudioStream *)pStream;
			
			refpAudioStream->nChannels = this->parserState.currentTrack.audio.nChannels;
			refpAudioStream->sampleRate = (uint32)this->parserState.currentTrack.audio.samplingFrequency;
			
			//check for PCM
			if(this->parserState.currentTrack.codecId == ECodecId::Unknown && this->parserState.currentTrack.codecPrivate.isFloatPCM)
			{
				switch(this->parserState.currentTrack.audio.bitDepth)
				{
				case 32:
					pStream->SetCodec(ECodecId::PCM_Float32LE);
					break;
				default:
					NOT_IMPLEMENTED_ERROR;
				}
			}
		}
		break;
	case TRACK_TYPE_VIDEO:
		{
			CVideoStream *const& refpVideoStream = (CVideoStream *)pStream;

			//check for microsoft BMP header
			if(this->parserState.currentTrack.codecId == ECodecId::Unknown && this->parserState.currentTrack.codecPrivate.isMS_BMPHeader)
			{
				bool isBottomUp;
				uint64 currentOffset;

				currentOffset = this->refInput.GetCurrentOffset();
				this->refInput.SetCurrentOffset(this->parserState.currentTrack.codecPrivate.offset);
				ReadBMPHeader(isBottomUp, this->refInput, *refpVideoStream);
				this->refInput.SetCurrentOffset(currentOffset);
			}
		}
		break;
	}
}

void CMatroskaDemuxer::BeginParseChilds(uint64 id)
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
			this->parserState.currentTrack.codecId = ECodecId::Unknown;
			this->parserState.currentTrack.codecPrivate.isMS_BMPHeader = false;
			this->parserState.currentTrack.codecPrivate.isFloatPCM = false;
			this->parserState.currentTrack.codecPrivate.isIntegerPCM = false;
		}
		break;
	}
}

uint64 CMatroskaDemuxer::DecodeVariableLengthInteger(uint8 &refLength)
{
	byte first, mask;
	uint64 result;

	result = 0;
	refLength = 1;
	mask = 0x80;
	
	first = this->refInput.ReadByte();
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
		
		result = (result << 8) | this->refInput.ReadByte();
		refLength++;
		mask >>= 1;
	}

	ASSERT(0);
	return result;
}

void CMatroskaDemuxer::EndParseChilds(uint64 id)
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

bool CMatroskaDemuxer::GetElementInfo(uint64 id, SElemInfo &refElemInfo)
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

ECodecId CMatroskaDemuxer::MapCodecId(const C8BitString &refString)
{
	//audio codecs
	if(refString == CODEC_PCM_FLOAT_LE)
	{
		this->parserState.currentTrack.codecPrivate.isFloatPCM = true;
		return ECodecId::Unknown;
	}

	if(refString == CODEC_PCM_INTEGER_LE)
	{
		this->parserState.currentTrack.codecPrivate.isIntegerPCM = true;
		return ECodecId::Unknown;
	}
	
	//video codecs
	if(refString == CODEC_MS_FOURCC)
	{
		this->parserState.currentTrack.codecPrivate.isMS_BMPHeader = true;
		return ECodecId::Unknown;
	}

	return MapCodecString(refString);
}

void CMatroskaDemuxer::ParseASCIIString(uint64 id, const C8BitString &refString)
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

void CMatroskaDemuxer::ParseBinary(uint64 id, uint64 size)
{
	switch(id)
	{
	case MATROSKA_ID_BLOCK:
	case MATROSKA_ID_SIMPLEBLOCK:
		{
			this->parserState.currentCluster.offsets.Push(this->refInput.GetCurrentOffset());
			this->parserState.currentCluster.sizes.Push(size);
			
			this->refInput.Skip(size);
		}
		break;
	case MATROSKA_ID_CODECPRIVATE:
		{
			this->parserState.currentTrack.codecPrivate.offset = this->refInput.GetCurrentOffset();
			this->parserState.currentTrack.codecPrivate.size = size;

			this->refInput.Skip(size);
		}
		break;
	default:
		this->refInput.Skip(size);
	}
}

uint64 CMatroskaDemuxer::ParseElement()
{
	uint8 headerLength, length;
	uint64 id, size;
	SElemInfo elemInfo;

	//read id
	id = this->DecodeVariableLengthInteger(length);
	id = PutLength(id, length);

	headerLength = length;

	//read size
	size = this->DecodeVariableLengthInteger(length);

	headerLength += length;
	
	if(this->refInput.HitEnd())
		return 0;
	
	if(this->GetElementInfo(id, elemInfo))
	{
		switch(elemInfo.type)
		{
		case EMatroskaType::ASCII_String:
			{
				C8BitString str;
				
				CTextReader reader(this->refInput);
				
				str = reader.ReadASCII(size);
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
				this->refInput.Skip(size);
			}
			break;
		case EMatroskaType::Float:
			{
				float64 value;
				
				if(size == 4)
					value = this->refInput.ReadFloat32BE();
				else
					value = this->refInput.ReadFloat64BE();
				
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
					value = (value << 8) | this->refInput.ReadByte();
				
				this->ParseUInt(id, value);
			}
			break;
		case EMatroskaType::UTF8:
			{
				this->refInput.Skip(size);
			}
			break;
		default:
			ASSERT(0);
		}
	}
	else
	{
		this->refInput.Skip(size);
	}

	return headerLength + size;
}

void CMatroskaDemuxer::ParseFloat(uint64 id, float64 value)
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

void CMatroskaDemuxer::ParseUInt(uint64 id, uint64 value)
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
void CMatroskaDemuxer::ReadHeader()
{
	while(!this->refInput.HitEnd())
		this->ParseElement();

	//move to beginning of data
	this->refInput.SetCurrentOffset(this->clusters.GetStartOffset());
}

bool CMatroskaDemuxer::ReadPacket(SPacket &refPacket)
{
	uint32 clusterIndex;

	while(true)
	{
		if(this->clusters.FindEntry(this->refInput.GetCurrentOffset(), clusterIndex))
		{
			uint8 length, flags;
			int16 timeCode;
			uint64 trackNumber;

			const CClusterEntry &refEntry = this->clusters.GetCluster(clusterIndex);

			trackNumber = this->DecodeVariableLengthInteger(length);
			timeCode = this->refInput.ReadInt16BE();
			flags = this->refInput.ReadByte();

			switch((flags >> 5) & 3)
			{
			case 0: //no lacing
				{
					InitPacket(refPacket);
					
					refPacket.size = refEntry.GetRemainingBytes(this->refInput.GetCurrentOffset());
					refPacket.pData = (byte *)MemAlloc(refPacket.size);
					this->refInput.ReadBytes(refPacket.pData, refPacket.size);
					
					refPacket.streamIndex = this->trackToStreamMap[trackNumber];
					refPacket.pts = refEntry.timeStamp + timeCode;
					refPacket.containsKeyframe = (flags & 0x80) != 0;
					
					return true;
				}
				break;
			default:
				ASSERT(0);
			}
		}
		else
		{
			//resync
			const CClusterEntry &refEntry = this->clusters.GetCluster(clusterIndex);
			
			if(refEntry.offset < this->refInput.GetCurrentOffset())
				clusterIndex++; //go to next cluster
			if(clusterIndex >= this->clusters.GetNumberOfClusters())
				break; //read all clusters... we're at the end
					   
			//move input stream to next cluster offset
			const CClusterEntry &refResyncEntry = this->clusters.GetCluster(clusterIndex);

			this->refInput.SetCurrentOffset(refResyncEntry.offset);
		}
	}
	
	return false;
}
//Class header
#include "CMPEGAudioDemuxer.h"
//Local
#include "../../../Codecs/MPEG/CMPEGAudioParser.h"
#include "../../../Codecs/MPEG/MPEGAudio.h"
#include "../../../FourCC.h"
#include "../../../ID3/ID3.h"
//Definitions
#define RAW_MPEG_AUDIO_PACKETSIZE 1024

//Private methods
void CMPEGAudioDemuxer::ReadVBRTags(const SMPEGAudioFrameHeader &refFrameHeader)
{
	uint16 samplesPerFrame;
	uint32 tag;

	if(refFrameHeader.isMpeg1)
		samplesPerFrame = 1152;
	else
		samplesPerFrame = 576;
	
	this->refInput.Skip(refFrameHeader.sideInfoSize); //skip side info
	tag = this->refInput.ReadUInt32LE();
	if(tag == FOURCC("Xing"))
	{
		uint32 flags, nFrames, dataSize;

		AStream *const& refpStream = this->GetStream(0);

		//means that this is coded vbr
		
		nFrames = 0;
		dataSize = 0;

		flags = this->refInput.ReadUInt32BE();
		if(flags & 1)
			nFrames = this->refInput.ReadUInt32BE();
		
		if(flags & 2)
			dataSize = this->refInput.ReadUInt32BE();

		refpStream->vbr = true;
		if(nFrames)
		{
			refpStream->duration = (uint64)nFrames / refpStream->timeScale * samplesPerFrame / refFrameHeader.sampleRate;
			
			if(dataSize)
				refpStream->bitRate = (uint64)dataSize * 8 * refFrameHeader.sampleRate / (nFrames * samplesPerFrame);
		}
	}
}

//Public methods
void CMPEGAudioDemuxer::ReadHeader()
{
	uint32 currentOffset, state;
	uint64 totalPayloadSize;
	AStream *pStream;
	
	pStream = new CAudioStream;
	pStream->SetParser(new CMPEGAudioParser);
	pStream->startTime = 0;
	pStream->timeScale = CFraction(1, 14112000); //least common multiple of all mpeg audio sample rates

	this->AddStream(pStream);
	
	ReadID3Tags(this->refInput, this->metaInfo);

	//read first frame in order to read VBR tags or to compute duration
	currentOffset = this->refInput.GetCurrentOffset();
	totalPayloadSize = this->refInput.GetRemainingBytes();
	state = 0;
	if(FindNextFrameHeader(state, this->refInput))
	{
		SMPEGAudioFrameHeader header;
		
		ParseFrameHeader(state, header);

		this->ReadVBRTags(header);

		if(pStream->duration == UINT64_MAX)
			pStream->duration = totalPayloadSize * 8 / pStream->timeScale / header.bitRate;
	}
	
	//reset
	this->refInput.SetCurrentOffset(currentOffset);
}

bool CMPEGAudioDemuxer::ReadPacket(SPacket &refPacket)
{
	uint32 left;
	
	if(this->refInput.HitEnd() || this->refInput.GetRemainingBytes() == 0)
		return false;
	
	left = RAW_MPEG_AUDIO_PACKETSIZE;
	if(this->refInput.GetRemainingBytes() < left)
		left = this->refInput.GetRemainingBytes();

	InitPacket(refPacket);
	
	refPacket.pData = (byte *)MemAlloc(left);
	refPacket.size = left;
	refPacket.streamIndex = 0;

	this->refInput.ReadBytes(refPacket.pData, left);

	return true;
}
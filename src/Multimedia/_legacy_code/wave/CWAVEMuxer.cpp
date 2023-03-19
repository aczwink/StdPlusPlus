//Class header
#include "CWAVEMuxer.h"
//Local
#include "WAVE.h"

//Constructor
CWAVEMuxer::CWAVEMuxer(const IFormat &refFormat, ASeekableOutputStream &refOutput) : AMuxer(refFormat, refOutput)
{
}

//Private methods
uint16 CWAVEMuxer::GetBitsPerSample(ECodecId codecId) const
{
	switch(codecId)
	{
	case ECodecId::PCM_Float32LE:
		return 32;
	case ECodecId::PCM_S16LE:
		return 16;
	}
	
	ASSERT(0);
	return 0;
}

//Public methods
void CWAVEMuxer::Finalize()
{
	uint32 currentOffset;

	currentOffset = (uint32)this->refOutput.GetCurrentOffset();
	this->refOutput.SetCurrentOffset(this->riffTagSizeOffset);
	this->refOutput.WriteUInt32LE(currentOffset - 8); //sizeof("RIFF") + sizeof(chunkSize)
	
	this->refOutput.SetCurrentOffset(this->dataChunkSizeOffset);
	this->refOutput.WriteUInt32LE(currentOffset - this->dataChunkSizeOffset - 4);
}

void CWAVEMuxer::WriteHeader()
{
	uint16 bitsPerSample, blockAlign;
	CAudioStream *pStream;
	
	pStream = (CAudioStream *)this->GetStream(0);
	
	bitsPerSample = this->GetBitsPerSample(pStream->GetCodec()->GetId());
	blockAlign = pStream->nChannels * bitsPerSample / 8;

	//riff chunk
	this->refOutput.WriteUInt32LE(WAVE_RIFFCHUNK_CHUNKID);
	this->riffTagSizeOffset = (uint32)this->refOutput.GetCurrentOffset();
	this->refOutput.WriteUInt32LE(0); //file size
	this->refOutput.WriteUInt32LE(WAVE_RIFFCHUNK_RIFFTYPE);

	//format chunk
	this->refOutput.WriteUInt32LE(WAVE_FORMATCHUNK_CHUNKID);
	this->refOutput.WriteUInt32LE(16); //size of format chunk
	this->refOutput.WriteUInt16LE(MapToTwoCC(pStream->GetCodec()->GetId()));
	this->refOutput.WriteUInt16LE(pStream->nChannels);
	this->refOutput.WriteUInt32LE(pStream->sampleRate);
	this->refOutput.WriteUInt32LE(pStream->sampleRate * blockAlign);
	this->refOutput.WriteUInt16LE(blockAlign);
	this->refOutput.WriteUInt16LE(bitsPerSample);
	
	//data chunk
	this->refOutput.WriteUInt32LE(WAVE_DATACHUNK_CHUNKID);
	this->dataChunkSizeOffset = (uint32)this->refOutput.GetCurrentOffset();
	this->refOutput.WriteUInt32LE(0); //chunk size
}

void CWAVEMuxer::WritePacket(const SPacket &refPacket)
{
	this->refOutput.WriteBytes(refPacket.pData, refPacket.size);
}
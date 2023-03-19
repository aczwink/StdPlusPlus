//Class header
#include "CPCM_Float32LE_Decoder.h"

//Public methods
void CPCM_Float32LE_Decoder::Decode(const SPacket &refPacket)
{
	uint32 nSamplesPerChannel, i, ch;
	CAudioBuffer<float32> *pAudioBuffer;
	
	CBufferInputStream input(refPacket.pData, refPacket.size);
	
	nSamplesPerChannel = refPacket.size / this->refStream.nChannels / sizeof(float32);
	
	pAudioBuffer = new CAudioBuffer<float32>(this->refStream.GetChannelLayout(), nSamplesPerChannel);
	for(i = 0; i < nSamplesPerChannel; i++)
	{
		for(ch = 0; ch < this->refStream.nChannels; ch++)
		{
			pAudioBuffer->GetChannel((EChannel)ch)[i] = input.ReadFloat32LE();
		}
	}

	this->AddFrame(new CAudioFrame(pAudioBuffer));
}
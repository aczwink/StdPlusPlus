//Class header
#include "CMPEGAudioParser.h"
//Local
#include "MPEGAudio.h"

//Constructor
CMPEGAudioParser::CMPEGAudioParser()
{
	this->layer = UINT8_MAX;
	
	this->Reset();
}

//Public methods
ECodecId CMPEGAudioParser::GetCodecId() const
{
	switch(this->layer)
	{
	case 2:
		return ECodecId::MP2;
	case 3:
		return ECodecId::MP3;
	}
	return ECodecId::Unknown;
}

void CMPEGAudioParser::Parse(const SPacket &refPacket)
{
	SPacket tmp;
	
	if(this->leftFrameSize)
	{
		if(this->leftFrameSize > refPacket.size)
		{
			this->AddToFrameBuffer(refPacket);
			this->leftFrameSize -= refPacket.size;
		}
		else
		{
			//add rest of frame
			tmp = refPacket;
			tmp.size = this->leftFrameSize;
			this->AddToFrameBuffer(tmp);
			this->frameBuffer.containsKeyframe = true; //all mpeg audio frames can be decoded independently
			this->ReadyFrameBuffer();

			//parse rest of packet
			tmp = refPacket;
			tmp.pData += this->leftFrameSize;
			tmp.size -= this->leftFrameSize;
			this->leftFrameSize = 0;
			
			this->Parse(tmp);
		}
	}
	else
	{
		uint32 offset;
		SMPEGAudioFrameHeader header;
		
		CBufferInputStream &refInput = CBufferInputStream(refPacket.pData, refPacket.size);
		
		if(FindNextFrameHeader(this->state, refInput))
		{
			ParseFrameHeader(this->state, header);
			
			if(this->layer == UINT8_MAX)
				this->layer = header.layer;
			ASSERT(this->layer == header.layer); //layer shouldn't change over time
			
			this->leftFrameSize = header.frameSize - 4; //frame size minus frame header that we already have read

			//add the frame header first
			this->AddFrameHeader(this->state);
			this->state = 0;

			//now parse the rest of the packet
			offset = (uint32)refInput.GetCurrentOffset();
			tmp = refPacket;
			tmp.pData += offset;
			tmp.size -= offset;

			this->Parse(tmp);
		}
		else
		{
			//the only case where this is legitimate is when we are at the end of the buffer
			ASSERT(refInput.HitEnd());
		}
	}
}

void CMPEGAudioParser::Reset()
{
	AParser::Reset();

	this->state = 0;
	this->leftFrameSize = 0;
}
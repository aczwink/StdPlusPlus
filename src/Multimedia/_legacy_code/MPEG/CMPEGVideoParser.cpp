//Class header
#include "CMPEGVideoParser.h"
//Local
#include "MPEG1.h"

//Constructor
CMPEGVideoParser::CMPEGVideoParser()
{
	this->codecId = ECodecId::Unknown;

	this->Reset();
}

//Private methods
bool CMPEGVideoParser::FindFrameEnd(const byte *pBuffer, uint32 bufferSize, int32 &refFrameEndOffset)
{
	byte startCode;
	CBufferInputStream bufferStream(pBuffer, bufferSize);

	while(true)
	{
		if(!FindNextStartCode(this->state, bufferStream))
		{
			break;
		}
		//found a start code...
		startCode = this->state & 0xFF; //get only the start code
		//reset the state, as in the next 4 bytes, there can't be a new one
		this->state = UINT32_MAX;

		//in case we find in between a sequence and a picture header an extension header, the stream is mpeg2 else mpeg1
		switch(startCode)
		{
		case MPEG1VIDEO_STARTCODE_SEQUENCE_HEADER:
			this->foundSequenceHeader = true;
			break;
		case MPEG1VIDEO_STARTCODE_EXTENSION:
			this->codecId = ECodecId::MPEG2Video;
			break;
		case MPEG1VIDEO_STARTCODE_PICTURE:
			{
				if(this->foundSequenceHeader && this->codecId == ECodecId::Unknown)
					this->codecId = ECodecId::MPEG1Video;
			}
			break;
		}

		if(!this->foundFrameStart && startCode >= MPEG1VIDEO_STARTCODE_SLICE_FIRST && startCode <= MPEG1VIDEO_STARTCODE_SLICE_LAST)
		{
			this->foundFrameStart = true;
		}

		if(this->foundFrameStart && (startCode < MPEG1VIDEO_STARTCODE_SLICE_FIRST || startCode > MPEG1VIDEO_STARTCODE_SLICE_LAST))
		{
			this->foundFrameStart = false;

			refFrameEndOffset = (uint32)bufferStream.GetCurrentOffset() - 4;

			return true;
		}
		
		if(startCode == MPEG1VIDEO_STARTCODE_PICTURE)
		{
			//check for I-frame
			if(bufferStream.GetRemainingBytes() > 1)
			{
				if(((pBuffer[bufferStream.GetCurrentOffset() + 1] >> 3) & 3) == 1) //I-Frame
					this->frameBuffer.containsKeyframe = true;
			}
		}
	}

	return false;
}

//Public methods
ECodecId CMPEGVideoParser::GetCodecId() const
{
	return this->codecId;
}

void CMPEGVideoParser::Parse(const SPacket &refPacket)
{
	int32 frameEndOffset;
	
	if(!this->FindFrameEnd(refPacket.pData, refPacket.size, frameEndOffset))
	{
		this->AddToFrameBuffer(refPacket);
		
		return;
	}

	ASSERT(frameEndOffset >= -4);

	if(frameEndOffset < 0)
	{
		/*
		We put a part of the next frame into the buffer...
		This can happen if only a part of the start code is at the end of a packet.
		This is then put into the framebuffer without knowing that it is a start code.
		Ready the frame without these bytes, and put the overread ones
		into the next buffer
		*/
		this->ReadyFrameBuffer(-frameEndOffset);
		
		//We need to parse the hole input again
		this->Parse(refPacket);
	}
	else
	{
		SPacket tmp;

		tmp = refPacket;
		tmp.size = frameEndOffset;

		this->AddToFrameBuffer(tmp);
		this->ReadyFrameBuffer();

		//Parse the rest of the input
		tmp = refPacket;
		tmp.pData += frameEndOffset;
		tmp.size -= frameEndOffset;
		this->Parse(tmp);
	}
}

void CMPEGVideoParser::Reset()
{
	AParser::Reset();

	this->state = UINT32_MAX;
	this->foundFrameStart = false;
	this->foundSequenceHeader = false;
}
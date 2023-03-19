//Class header
#include "CTGADemuxer.h"

//Public methods
void CTGADemuxer::ReadHeader()
{
	byte imgIdLength, colorMapType, codecType, pixelSize, imageDescriptor;
	CVideoStream *pStream;
	AVideoDecoder *pDecoder;

	//set stream
	pStream = new CVideoStream;
	this->AddStream(pStream);

	//read header data
	imgIdLength = refInput.ReadByte();
	colorMapType = refInput.ReadByte();
	codecType = refInput.ReadByte();
	refInput.Skip(5); //color map spec
	refInput.Skip(4); //x-origin and y-origin
	pStream->width = refInput.ReadUInt16LE();
	pStream->height = refInput.ReadUInt16LE();
	pixelSize = refInput.ReadByte();
	imageDescriptor = refInput.ReadByte();
	
	ASSERT(colorMapType == 0);

	refInput.Skip(imgIdLength);

	//RLE
	switch(codecType)
	{
	case 10:
		this->isRunLengthEncoded = true;
		break;
	default:
		this->isRunLengthEncoded = false;
	}

	//codec
	switch(codecType)
	{
	case 2:
	case 10:
		{
			this->isTopDown = (imageDescriptor & 0x20) != 0;

			switch(pixelSize)
			{
			case 24:
				{
					pStream->SetCodec(ICodec::GetCodec(ECodecId::BGR24));
				}
				break;
			case 32:
				{
					pStream->SetCodec(ICodec::GetCodec(ECodecId::BGRA32));
				}
				break;
			default:
				NOT_IMPLEMENTED_ERROR;
			}
		}
		break;
	default:
		NOT_IMPLEMENTED_ERROR;
	}
}

bool CTGADemuxer::ReadPacket(SPacket &refPacket)
{
	uint8 pixelSize;
	uint32 rowSize, imageSize;
	CVideoStream *pStream;
	
	//check if image hasn't been read already
	if(!refInput.GetRemainingBytes() || refInput.HitEnd())
		return false;
	
	pStream = (CVideoStream *)this->GetStream(0);
	
	//calc image size
	switch(pStream->GetCodec()->GetId())
	{
	case ECodecId::BGR24:
		pixelSize = 3;
		break;
	case ECodecId::BGRA32:
		pixelSize = 4;
		break;
	}
	rowSize = pixelSize * pStream->width;
	imageSize = pStream->height * rowSize;
	
	//fill out packet
	refPacket.pts = 0;
	refPacket.streamIndex = 0;

	refPacket.size = (uint32)imageSize;
	refPacket.pData = (byte *)MemAlloc(refPacket.size);
	
	if(this->isRunLengthEncoded)
	{
		byte pixel[4];
		uint8 header, length;
		uint32 left;
		
		CBufferedInputStream bufferedInput(this->refInput); //buffering improves speed here very much
		CBufferOutputStream bufferOutput(refPacket.pData, refPacket.size);
		
		left = pStream->width * pStream->height;
		while(left)
		{
			header = bufferedInput.ReadByte();
			length = (header & 0x7F) + 1;

			if(header & 0x80)
				bufferedInput.ReadBytes(pixel, pixelSize);
			
			while(left && length)
			{
				if(!(header & 0x80))
					bufferedInput.ReadBytes(pixel, pixelSize);
				
				bufferOutput.WriteBytes(pixel, pixelSize);

				left--;
				length--;
			}
		}
	}
	else
	{
		refInput.ReadBytes(refPacket.pData, refPacket.size);
	}
	
	if(!this->isTopDown)
	{
		uint32 i;
		byte *pBuffer;

		//swap image
		pBuffer = (byte *)MemAlloc(refPacket.size);
		for(i = 0; i < pStream->height; i++)
			MemCopy(&pBuffer[i * rowSize], &refPacket.pData[(pStream->height - 1 - i) * rowSize], rowSize);
		
		MemFree(refPacket.pData);
		refPacket.pData = pBuffer;
	}
	
	refInput.Skip((uint32)refInput.GetRemainingBytes()); //if there is any stupid data left skip it
	
	return true;
}
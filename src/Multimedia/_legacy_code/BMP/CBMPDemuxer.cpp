//Class Header
#include "CBMPDemuxer.h"
//Local
#include "BMP.h"

//Public methods
void CBMPDemuxer::ReadHeader()
{
	CVideoStream *pStream;
	
	this->refInput.Skip(14); //Skip the bitmap file header
	
	pStream = new CVideoStream;
	ReadBMPHeader(this->isBottomUp, refInput, *pStream);
	
	this->AddStream(pStream);
}

bool CBMPDemuxer::ReadPacket(SPacket &refPacket)
{
	CVideoStream *pStream;
	
	if(!refInput.GetRemainingBytes() || refInput.HitEnd())
		return false;
	
	pStream = (CVideoStream *)this->GetStream(0);
	
	//fill out packet
	refPacket.pts = 0;
	refPacket.streamIndex = 0;
	
	refPacket.size = (uint32)refInput.GetRemainingBytes();
	refPacket.pData = (byte *)MemAlloc(refPacket.size);

	if(this->isBottomUp)
	{
		byte *pBuffer;
		uint32 rowSize, rowSizeWithPadding, imageSize, dataSize, sizeDifference, i, nRows;

		rowSize = 3 * pStream->width;
		rowSizeWithPadding = ((24 * pStream->width + 31) / 32) * 4;
		imageSize = rowSize * pStream->height;
		dataSize = (uint32)refInput.GetRemainingBytes();
		if(imageSize > dataSize)
		{
			//the image is cut :S
			sizeDifference = imageSize - dataSize;
			refPacket.size = dataSize; //shrink the packet
		}
		else
		{
			//there is more (or equal) data than the image is large
			refPacket.size = imageSize; //shrink (or keep) the packet
		}
		nRows = refPacket.size / rowSizeWithPadding; //image may be cut...
		pBuffer = refPacket.pData + refPacket.size - rowSize;

		for(i = 0; i < nRows; i++)
		{
			refInput.ReadBytes(pBuffer, rowSize);
			refInput.Skip(rowSizeWithPadding - rowSize);
			pBuffer -= rowSize;
		}

		refInput.Skip((uint32)refInput.GetRemainingBytes());
	}
	else
	{
		refInput.ReadBytes(refPacket.pData, refPacket.size);
	}
	
	return true;
}
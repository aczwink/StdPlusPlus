//Class Header
#include "CBMPMuxer.h"
//Local
#include "BMP.h"
//Definitions
#define BMP_FILEHEADER_SIZE 14

//Public methods
void CBMPMuxer::Finalize()
{
	uint64 currentOffset;
	
	currentOffset = this->refOutput.GetCurrentOffset();
	
	//Update file size
	this->refOutput.SetCurrentOffset(this->startOffset + BMP_HEADER_TYPE_SIZE);
	this->refOutput.WriteUInt32LE((uint32)(currentOffset - this->startOffset));

	//Update image size
	this->refOutput.SetCurrentOffset(this->startOffset + BMP_FILEHEADER_SIZE + 20);
	this->refOutput.WriteUInt32LE(this->imageSize);
}

void CBMPMuxer::WriteHeader()
{
	CVideoStream *pStream;

	pStream = (CVideoStream *)this->GetStream(0);
	
	this->startOffset = this->refOutput.GetCurrentOffset();
	
	//bitmap file header
	this->refOutput.WriteBytes(BMP_HEADER_TYPE, BMP_HEADER_TYPE_SIZE);
	this->refOutput.WriteUInt32LE(0); //file size... updated in finalize
	this->refOutput.WriteUInt16LE(0); //reserved1
	this->refOutput.WriteUInt16LE(0); //reserved2
	this->refOutput.WriteUInt32LE(BMP_FILEHEADER_SIZE + BMP_INFOHEADER_SIZE); //data offset

	//bitmap info header
	this->refOutput.WriteUInt32LE(BMP_INFOHEADER_SIZE);
	this->refOutput.WriteUInt32LE(pStream->width);
	this->refOutput.WriteUInt32LE(-(int32)pStream->height);
	this->refOutput.WriteUInt16LE(1); //number of planes
	this->refOutput.WriteUInt16LE(24); //bits per pixel
	this->refOutput.WriteUInt32LE(0); //compression
	this->refOutput.WriteUInt32LE(0); //image size... updated in finalize
	this->refOutput.WriteUInt32LE(0); //pixels per meter horizontal
	this->refOutput.WriteUInt32LE(0); //pixels per meter vertical
	this->refOutput.WriteUInt32LE(0); //number of color indexes
	this->refOutput.WriteUInt32LE(0); //biClrImportant
}

void CBMPMuxer::WritePacket(const SPacket &refPacket)
{
	CVideoStream *pStream;
	
	pStream = (CVideoStream *)this->GetStream(0);
	
	switch(pStream->GetCodec()->GetId())
	{
	case ECodecId::BGR24:
		{
			uint16 nRows, rowSize;
			byte *pCurrent;

			pCurrent = refPacket.pData;
			nRows = refPacket.size / 3 / pStream->height;
			rowSize = 3 * pStream->width;
			for(uint16 y = 0; y < nRows; y++)
			{
				this->refOutput.WriteBytes(pCurrent, rowSize);

				//write padding
				for(uint8 p = 0; p < rowSize % 4; p++)
					this->refOutput.WriteByte(0);

				pCurrent += rowSize;
			}
		}
		break;
	
	default:
		this->refOutput.WriteBytes(refPacket.pData, refPacket.size);
		this->imageSize += refPacket.size;
	}
}
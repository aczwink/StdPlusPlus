//Class Header
#include "CDDSDemuxer.h"
//Local
#include "../../FourCC.h"

//Public methods
void CDDSDemuxer::ReadHeader()
{
	CVideoStream *pStream;
	
	pStream = new CVideoStream;

	this->refInput.Skip(4); //dwMagic "DDS "
	this->refInput.Skip(4); //dwSize == 124
	this->refInput.Skip(4); //dwFlags
	pStream->height = (uint16)this->refInput.ReadUInt32LE();
	pStream->width = (uint16)this->refInput.ReadUInt32LE();
	this->refInput.Skip(4); //dwPitchOrLinearSize
	this->refInput.Skip(4); //dwDepth
	this->refInput.Skip(4); //dwMipMapCount
	this->refInput.Skip(44); //dwReserved1[11]

	//read DDS_PIXELFORMAT
	this->refInput.Skip(4); //dwSize == 32
	this->refInput.Skip(4); //dwFlags
	switch(this->refInput.ReadUInt32LE())
	{
	case FOURCC("DXT1"):
		pStream->SetCodec(ECodecId::S3TC_DXT1);
		break;
	default:
		NOT_IMPLEMENTED_ERROR;
	}
	this->refInput.Skip(4); //dwRGBBitCount
	this->refInput.Skip(4); //dwRBitMask
	this->refInput.Skip(4); //dwGBitMask
	this->refInput.Skip(4); //dwBBitMask
	this->refInput.Skip(4); //dwABitMask

	this->refInput.Skip(4); //dwCaps
	this->refInput.Skip(4); //dwCaps2
	this->refInput.Skip(4); //dwCaps3
	this->refInput.Skip(4); //dwCaps4
	this->refInput.Skip(4); //dwReserved2
	
	this->AddStream(pStream);
}

bool CDDSDemuxer::ReadPacket(SPacket &refPacket)
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
	
	refInput.ReadBytes(refPacket.pData, refPacket.size);
	
	return true;
}
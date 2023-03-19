//Class Header
#include "CPPMDemuxer.h"

//Private methods
uint16 CPPMDemuxer::ReadUInt()
{
	byte b;
	CString buffer;
	
	while(true)
	{
		b = this->refInput.ReadByte();
		switch(b)
		{
		case '\n':
			goto end;
		}
		
		buffer += (char)b;
	}

end:;
	return (uint16)StringToUInt64(buffer);
}

//Public methods
void CPPMDemuxer::ReadHeader()
{
	CVideoStream *pStream;
	
	CTextReader reader(this->refInput);

	//set stream
	pStream = new CVideoStream;
	this->AddStream(pStream);
	pStream->SetCodec(ICodec::GetCodec(ECodecId::RGB24));
	
	reader.ReadASCII_Line(); //first line contains header
	pStream->width = this->ReadUInt();
	pStream->height = this->ReadUInt();
	this->ReadUInt(); //unnecessary
}

bool CPPMDemuxer::ReadPacket(SPacket &refPacket)
{
	uint32 imageSize;
	CVideoStream *pStream;
	
	//check if image hasn't been read already
	if(!refInput.GetRemainingBytes() || refInput.HitEnd())
		return false;

	pStream = (CVideoStream *)this->GetStream(0);
	
	//calc image size
	imageSize = 3 * pStream->width * pStream->height;
	
	//fill out packet
	refPacket.pts = 0;
	refPacket.streamIndex = 0;
	
	refPacket.size = (uint32)imageSize;
	refPacket.pData = (byte *)MemAlloc(refPacket.size);
	
	refInput.ReadBytes(refPacket.pData, refPacket.size);
	
	refInput.Skip((uint32)refInput.GetRemainingBytes()); //if there is any stupid data left skip it
	
	return true;
}
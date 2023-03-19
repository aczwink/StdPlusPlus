//Class header
#include "CBGR24Decoder.h"

//Constructor
CBGR24Decoder::CBGR24Decoder(AStream &refStream) : AVideoDecoder(refStream)
{
}

//Public methods
void CBGR24Decoder::Decode(const SPacket &refPacket)
{
	uint32 i;
	byte *pR, *pG, *pB, *pBGR;
	CR8G8B8Image *pImage;
	
	pImage = new CR8G8B8Image(this->refStream.width, this->refStream.height);
	
	pR = pImage->GetRedChannel();
	pG = pImage->GetGreenChannel();
	pB = pImage->GetBlueChannel();
	pBGR = refPacket.pData;
	
	for(i = refPacket.size; i > 3; i -= 3)
	{
		*pB++ = *pBGR++;
		*pG++ = *pBGR++;
		*pR++ = *pBGR++;
	}
	
	//theoretically 2 bytes could overlap but this would imply an uncomplete color info so just ignore
	
	this->AddFrame(new CVideoFrame(pImage));
}

EPixelFormat CBGR24Decoder::GetPixelFormat() const
{
	return EPixelFormat::R8G8B8;
}
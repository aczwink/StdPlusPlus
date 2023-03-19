//Class header
#include "CBGRA32Decoder.h"

//Constructor
CBGRA32Decoder::CBGRA32Decoder(AStream &refStream) : AVideoDecoder(refStream)
{
}

//Public methods
void CBGRA32Decoder::Decode(const SPacket &refPacket)
{
	uint32 i;
	byte *pR, *pG, *pB, *pA, *pBGRA;
	CR8G8B8A8Image *pImage;
	
	pImage = new CR8G8B8A8Image(this->refStream.width, this->refStream.height);

	pR = pImage->GetRedChannel();
	pG = pImage->GetGreenChannel();
	pB = pImage->GetBlueChannel();
	pA = pImage->GetAlphaChannel();
	pBGRA = refPacket.pData;
	
	for(i = refPacket.size; i >= 4; i -= 4)
	{
		*pB++ = *pBGRA++;
		*pG++ = *pBGRA++;
		*pR++ = *pBGRA++;
		*pA++ = *pBGRA++;
	}
	
	//theoretically 3 bytes could overlap but this would imply an uncomplete color info so just ignore
	
	this->AddFrame(new CVideoFrame(pImage));
}

EPixelFormat CBGRA32Decoder::GetPixelFormat() const
{
	return EPixelFormat::R8G8B8A8;
}
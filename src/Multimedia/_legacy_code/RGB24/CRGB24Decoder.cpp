//Class header
#include "CRGB24Decoder.h"

//Public methods
void CRGB24Decoder::Decode(const SPacket &refPacket)
{
	uint32 i;
	byte *pR, *pG, *pB, *pRGB;
	CR8G8B8Image *pImage;

	pImage = new CR8G8B8Image(this->refStream.width, this->refStream.height);

	pR = pImage->GetRedChannel();
	pG = pImage->GetGreenChannel();
	pB = pImage->GetBlueChannel();
	pRGB = refPacket.pData;

	for(i = refPacket.size; i > 3; i -= 3)
	{
		*pR++ = *pRGB++;
		*pG++ = *pRGB++;
		*pB++ = *pRGB++;
	}

	//theoretically 2 bytes could overlap but this would imply an uncomplete color info so just ignore

	this->AddFrame(new CVideoFrame(pImage));
}

EPixelFormat CRGB24Decoder::GetPixelFormat() const
{
	return EPixelFormat::R8G8B8;
}
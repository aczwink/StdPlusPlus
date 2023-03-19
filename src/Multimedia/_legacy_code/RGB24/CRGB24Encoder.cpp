//Class header
#include "CRGB24Encoder.h"

//Public methods
void CRGB24Encoder::Encode(const AFrame &refFrame, SPacket &refPacket) const
{
	bool deleteRGBImage;
	byte r, g, b;
	byte *pRGB;
	AImage *pImage;
	ARGBImage *pRGBImage;
	
	ASSERT(refFrame.GetType() == EDataType::Video);
	
	CVideoFrame &refVideoFrame = (CVideoFrame &)refFrame;

	pImage = refVideoFrame.GetImage();

	//convert to RGB if neccessary
	if(pImage->GetColorSpace() == EColorSpace::RGB)
	{
		pRGBImage = (ARGBImage *)pImage;
		deleteRGBImage = false;
	}
	else
	{
		pRGBImage = (ARGBImage *)pImage->Resample(EColorSpace::RGB);
		deleteRGBImage = true;
	}

	//fill out packet
	InitPacket(refPacket);
	
	refPacket.size = 3 * pRGBImage->GetNumberOfPixels();
	refPacket.pData = (byte *)MemAlloc(refPacket.size);
	refPacket.pts = refFrame.pts;

	//fill data
	pRGB = refPacket.pData;
	for(uint32 i = 0; i < pRGBImage->GetNumberOfPixels(); i++)
	{
		pRGBImage->GetPixel(i, r, g, b);

		*pRGB++ = r;
		*pRGB++ = g;
		*pRGB++ = b;
	}

	//clean up
	if(deleteRGBImage)
		delete pRGBImage;
}
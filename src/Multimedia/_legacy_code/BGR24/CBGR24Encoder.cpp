//Class header
#include "CBGR24Encoder.h"

//Public methods
void CBGR24Encoder::Encode(const AFrame &refFrame, SPacket &refPacket) const
{
	byte r, g, b;
	byte *pBGR;
	ARGBImage *pRGBImage;
	
	CVideoFrame &refVideoFrame = (CVideoFrame &)refFrame;

	//get image
	if(refVideoFrame.GetImage()->GetColorSpace() == EColorSpace::RGB)
	{
		pRGBImage = (ARGBImage *)refVideoFrame.GetImage();
	}
	else
	{
		pRGBImage = (ARGBImage *)refVideoFrame.GetImage()->Resample(EColorSpace::RGB);
	}

	//encode
	refPacket.size = 3 * pRGBImage->GetNumberOfPixels();
	refPacket.pData = (byte *)MemAlloc(refPacket.size);
	
	pBGR = refPacket.pData;
	for(uint32 i = 0; i < pRGBImage->GetNumberOfPixels(); i++)
	{
		pRGBImage->GetPixel(i, r, g, b);
		
		*pBGR++ = b;
		*pBGR++ = g;
		*pBGR++ = r;
	}

	//clean up
	if(refVideoFrame.GetImage() != pRGBImage)
	{
		delete pRGBImage; //the frame that we converted
	}
}
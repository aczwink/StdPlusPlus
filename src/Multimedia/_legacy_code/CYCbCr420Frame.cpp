//Class Header
#include "CYCbCr420Frame.h"
//Global
#include <iostream>
//Local
#include "CR8G8B8Frame.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

//Constructor
CYCbCr420Frame::CYCbCr420Frame(uint16 width, uint16 height)
{	
	this->aspectRatio.Init(width, height);
	this->aspectRatio.Reduce();
}

//Public Functions
AVideoFrame *CYCbCr420Frame::Convert(EPixelFormat pixelFormat) const
{
	switch(pixelFormat)
	{
	case PIXELFORMAT_R8G8B8:
		{
			CR8G8B8Frame *pFrame;
			const uint8 *pSrcY;
			const uint8 *pSrcCb;
			const uint8 *pSrcCr;
			uint8 *pDestR;
			uint8 *pDestG;
			uint8 *pDestB;

			pFrame = new CR8G8B8Frame(this->GetWidth(), this->GetHeight());
			pSrcY = this->pLumaChannel;
			pSrcCb = this->pChromaBlueChannel;
			pSrcCr = this->pChromaRedChannel;
			pDestR = pFrame->GetRedChannel();
			pDestG = pFrame->GetGreenChannel();
			pDestB = pFrame->GetBlueChannel();

			repeat(this->height, i)
			{
				for(uint32 j = 0; j < width; j += 2)
				{
					register int32 value;

					repeat(2, k)
					{
						value = int32(255.0 / 219.0 * (*pSrcY - 16) + 255.0 / 112.0 * 0.701 * (((int32)*pSrcCr) - 128));
						*pDestR++ = CLIP(value, 0, 0xFF);

						value = int32(255.0 / 219.0 * (*pSrcY - 16) - 255.0 / 112.0 * 0.886 * 0.114 / 0.587 * (((int32)*pSrcCb) - 128) - 255.0 / 112.0 * 0.701 * 0.299 / 0.587 * (((int32)*pSrcCr) - 128));
						*pDestG++ = CLIP(value, 0, 0xFF);

						value = int32(255.0 / 219.0 * (*pSrcY - 16) + 255.0 / 112.0 * 0.886 * (((int32)*pSrcCb) - 128));
						*pDestB++ = CLIP(value, 0, 0xFF);

						pSrcY++;
					}

					pSrcCb++;
					pSrcCr++;
				}

				if(!(i & 1))
				{
					pSrcCb -= width / 2;
					pSrcCr -= width / 2;
				}
			}

			return pFrame;
		}
		break;
	default:
		ASSERT(0);
	}

	return NULL;
}

CYCbCr420Frame *CYCbCr420Frame::Copy()
{
	CYCbCr420Frame *pFrame;

	pFrame = new CYCbCr420Frame(this->GetWidth(), this->GetHeight());
	
	MemCopy(pFrame->pLumaChannel, this->pLumaChannel, this->GetLumaChannelSize());
	MemCopy(pFrame->pChromaBlueChannel, this->pChromaBlueChannel, this->GetChromaBlueChannelSize());
	MemCopy(pFrame->pChromaRedChannel, this->pChromaRedChannel, this->GetChromaRedChannelSize());
	
	pFrame->isKeyFrame = this->isKeyFrame;

	return pFrame;
}

EPixelFormat CYCbCr420Frame::GetPixelFormat() const
{
	return PIXELFORMAT_YCBCR420;
}
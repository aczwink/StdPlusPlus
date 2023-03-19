//Class header
#include "CS3TCEncoder.h"

//Private methods
void CS3TCEncoder::EncodeColorBlock(const vec4i32(&refBlock)[16], AOutputStream &refOutput) const
{
	vec4i32 palette[4];

	//color block encoding for any DXT other than 1

	this->FindMinMaxColorsByEuclideanDistance(palette[1], palette[0], refBlock);

	palette[2] = (2 * palette[0] + palette[1]) / 3;
	palette[3] = (2 * palette[1] + palette[0]) / 3;

	refOutput.WriteUInt16LE(this->ToRGB565(palette[0]));
	refOutput.WriteUInt16LE(this->ToRGB565(palette[1]));
	refOutput.WriteUInt32LE(this->EncodeColorMask(refBlock, palette));
}

uint32 CS3TCEncoder::EncodeColorMask(const vec4i32(&refBlock)[16], const vec4i32(&refPalette)[4]) const
{
	uint8 i, j, colorIndex;
	uint32 pixelMask, closestDistance, distance;

	pixelMask = 0;

	for(i = 0; i < 16; i++)
	{
		colorIndex = 0;
		closestDistance = UINT32_MAX;

		//encode pixel mask
		for(j = 0; j < 4; j++)
		{
			distance = this->ComputeColorDistance(refPalette[j], refBlock[i]);
			if(distance < closestDistance)
			{
				colorIndex = j;
				closestDistance = distance;
			}
		}

		pixelMask |= colorIndex << (i * 2);
	}

	return pixelMask;
}

void CS3TCEncoder::EncodeDXT1Block(const vec4i32(&refBlock)[16], AOutputStream &refOutput) const
{
	vec4i32 palette[4];
	
	this->FindMinMaxColorsByEuclideanDistance(palette[1], palette[0], refBlock);

	if(this->ToRGB565(palette[0]) > this->ToRGB565(palette[1]))
	{
		palette[2] = (2 * palette[0] + palette[1]) / 3;
		palette[3] = (2 * palette[1] + palette[0]) / 3;
	}
	else
	{
		//can happen if palette[0] == palette[1]
		palette[2] = (palette[0] + palette[1]) / 2;
		palette[3] = vec4i32(); //zero
	}

	refOutput.WriteUInt16LE(this->ToRGB565(palette[0]));
	refOutput.WriteUInt16LE(this->ToRGB565(palette[1]));
	refOutput.WriteUInt32LE(this->EncodeColorMask(refBlock, palette));
}

void CS3TCEncoder::EncodeDXT5AlphaBlock(const byte(&refBlock)[16], AOutputStream &refOutput) const
{
	uint8 i, j, alphaIndex, closest, distance;
	uint64 alphaMask;
	byte alphaPalette[8];

	this->FindMinMaxAlpha(alphaPalette[1], alphaPalette[0], refBlock);

	//compute palette
	if(alphaPalette[0] > alphaPalette[1])
	{
		for(i = 1; i <= 6; i++)
			alphaPalette[i+1] = ((7 - i) * alphaPalette[0] + i * alphaPalette[1]) / 7;
	}
	else
	{
		for(i = 1; i <= 4; i++)
			alphaPalette[i + 1] = ((5 - i) * alphaPalette[0] + i * alphaPalette[1]) / 7;
		alphaPalette[6] = 0;
		alphaPalette[7] = 255;
	}

	//compute mask
	alphaMask = 0;
	for(i = 0; i < 16; i++)
	{
		alphaIndex = 0;
		closest = UINT8_MAX;
		
		for(j = 0; j < 8; j++)
		{
			distance = (uint8)ABS((int16)alphaPalette[j] - (int16)refBlock[i]);
			if(distance < closest)
			{
				alphaIndex = j;
				closest = distance;
			}
		}

		alphaMask |= alphaIndex << (i * 3);
	}
	
	//or first two palette values into mask
	alphaMask |= (uint64)alphaPalette[0] << 56;
	alphaMask |= (uint64)alphaPalette[1] << 48;
	
	refOutput.WriteUInt64LE(alphaMask);
}

void CS3TCEncoder::FindMinMaxAlpha(byte &refMin, byte &refMax, const byte(&refBlock)[16]) const
{
	uint8 i;
	
	refMax = refMin = refBlock[0];

	for(i = 1; i < 16; i++)
	{
		if(refBlock[i] < refMin)
			refMin = refBlock[i];
		if(refBlock[i] > refMax)
			refMax = refBlock[i];
	}
}

void CS3TCEncoder::FindMinMaxColorsByEuclideanDistance(vec4i32 &refMin, vec4i32 &refMax, const vec4i32(&refBlock)[16]) const
{
	uint8 i, j, minIndex, maxIndex;
	uint32 distance, maxDistance;

	//init in case all colors are black
	maxDistance = 0;
	minIndex = 0;
	maxIndex = 0;

	for(i = 0; i < 15; i++) //we don't need to check last one (because it was already checked because of symmetry)
	{
		for(j = i + 1; j < 16; j++)
		{
			distance = this->ComputeColorDistance(refBlock[i], refBlock[j]);
			if(distance > maxDistance)
			{
				minIndex = i;
				maxIndex = j;
				maxDistance = distance;
			}
		}
	}

	refMin = refBlock[minIndex];
	refMax = refBlock[maxIndex];

	if(this->ToRGB565(refMax) < this->ToRGB565(refMin))
	{
		vec4i32 tmp;

		tmp = refMin;
		refMin = refMax;
		refMax = tmp;
	}
}

//Public methods
void CS3TCEncoder::Encode(const AFrame &refFrame, SPacket &refPacket) const
{
	bool useAlpha, resampled;
	uint8 i, r, g, b, a;
	uint16 x, y;
	AImage *pImage;
	ARGBImage *pRGBImage;
	ARGBAImage *pRGBAImage;
	vec4i32 colorBlock[16];
	byte alphaBlock[16];

	pRGBAImage = nullptr;
	pRGBImage = nullptr;
	
	ASSERT(refFrame.GetType() == EDataType::Video);

	CVideoFrame &refVideoFrame = (CVideoFrame &)refFrame;

	pImage = refVideoFrame.GetImage();

	InitPacket(refPacket);

	//check color space
	if(this->dxtNum == 1)
	{
		switch(pImage->GetColorSpace())
		{
		case EColorSpace::RGB:
			{
				useAlpha = false;
				resampled = false;
				pRGBImage = (ARGBImage *)pImage;
			}
			break;
		case EColorSpace::RGBA:
			{
				useAlpha = true;
				resampled = false;
				pRGBAImage = (ARGBAImage *)pImage;
			}
			break;
		default:
			{
				//standard is rgb i would say
				pRGBImage = (ARGBImage *)pImage->Resample(EColorSpace::RGB);
				useAlpha = false;
				resampled = true;
			}
		}

		refPacket.size = pImage->GetNumberOfPixels() * 8 / 16; //8 byte for 16 pixels
	}
	else
	{
		useAlpha = true;
		
		if(pImage->GetColorSpace() == EColorSpace::RGBA)
		{
			resampled = false;
			pRGBAImage = (ARGBAImage *)pImage;
		}
		else
		{
			resampled = true;
			pRGBAImage = (ARGBAImage *)pImage->Resample(EColorSpace::RGBA);
		}

		refPacket.size = pImage->GetNumberOfPixels() * 16 / 16; //16 byte for 16 pixels
	}
	
	//fill out packet
	refPacket.pData = (byte *)MemAlloc(refPacket.size);
	refPacket.pts = refFrame.pts;
	
	CBufferOutputStream output(refPacket.pData, refPacket.size);
	
	//encode
	if(useAlpha)
	{
		for(y = 0; y < pRGBAImage->GetHeight(); y += 4)
		{
			for(x = 0; x < pRGBAImage->GetWidth(); x += 4)
			{
				for(i = 0; i < 16; i++)
				{
					pRGBAImage->GetPixel(x + (i % 4), y + (i / 4), r, g, b, a);
					colorBlock[i] = vec4i32(r, g, b, 0);
					alphaBlock[i] = a;
				}
				
				switch(this->dxtNum)
				{
				case 5:
					this->EncodeDXT5AlphaBlock(alphaBlock, output);
					this->EncodeColorBlock(colorBlock, output);
					break;
				default:
					NOT_IMPLEMENTED_ERROR;
				}
			}
		}
	}
	else
	{
		//this can only be dxt1
		for(y = 0; y < pRGBImage->GetHeight(); y += 4)
		{
			for(x = 0; x < pRGBImage->GetWidth(); x += 4)
			{
				for(i = 0; i < 16; i++)
				{
					pRGBImage->GetPixel(x + (i % 4), y + (i / 4), r, g, b);
					colorBlock[i] = vec4i32(r, g, b, 0);
				}

				this->EncodeDXT1Block(colorBlock, output);
			}
		}
	}

	//clean up
	if(resampled)
	{
		if(useAlpha)
			delete pRGBAImage;
		else
			delete pRGBImage;
	}
}
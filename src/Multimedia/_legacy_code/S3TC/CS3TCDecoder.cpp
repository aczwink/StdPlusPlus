//Class header
#include "CS3TCDecoder.h"

//Local functions
inline void UnpackRGB565(uint16 packed, vec4u32 &refUnpacked)
{
	refUnpacked.a = packed >> 11;
	refUnpacked.b = (packed >> 5) & 0x3F;
	refUnpacked.c = packed & 0x1F;

	refUnpacked.a *= (256 / 32);
	refUnpacked.b *= (256 / 64);
	refUnpacked.c *= (256 / 32);
}

//Public methods
void CS3TCDecoder::Decode(const SPacket &refPacket)
{
	CBufferInputStream input(refPacket.pData, refPacket.size);
	
	switch(this->dxtNum)
	{
	case 1:
		{
			uint8 i;
			uint16 c0, c1, x, y;
			uint32 pixels;
			vec4u32 c[4];
			CR8G8B8Image *pImage;
			
			pImage = new CR8G8B8Image(this->refStream.width, this->refStream.height);

			for(y = 0; y < this->refStream.height; y += 4)
			{
				for(x = 0; x < this->refStream.width; x += 4)
				{
					c0 = input.ReadUInt16LE();
					c1 = input.ReadUInt16LE();
					UnpackRGB565(c0, c[0]);
					UnpackRGB565(c1, c[1]);
					
					if(c0 > c1)
					{
						c[2] = (2 * c[0] + c[1]) / 3;
						c[3] = (2 * c[1] + c[0]) / 3;
					}
					else
					{
						c[2] = (c[0] + c[1]) / 2;
						c[3] = vec4u32(); //set to zero
					}
					pixels = input.ReadUInt32LE();

					for(i = 0; i < 16; i++)
					{
						pImage->SetPixel(x + (i % 4), this->refStream.height - 1 - (y + (i / 4)), c[pixels & 3].a, c[pixels & 3].b, c[pixels & 3].c);
						pixels >>= 2;
					}
				}
			}
			
			this->AddFrame(new CVideoFrame(pImage));
		}
		break;
	default:
		NOT_IMPLEMENTED_ERROR;
	}
}

EPixelFormat CS3TCDecoder::GetPixelFormat() const
{
	return EPixelFormat::R8G8B8;
}
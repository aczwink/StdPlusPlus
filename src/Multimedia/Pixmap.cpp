/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of Std++.
 *
 * Std++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Std++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Std++.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include <Std++/Multimedia/Pixmap.hpp>
//Local
#include <Std++/Debug.hpp>
#include <Std++/Mathematics.hpp>
#include <Std++/Memory.hpp>
#include <Std++/Unsigned.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::Multimedia;

//Destructor
Pixmap::~Pixmap()
{
	//release planes
	for(uint8 i = 0; i < this->pixelFormat.nPlanes; i++)
		MemFree(this->planes[i]);
}

//Private methods
void Pixmap::Allocate()
{
	//allocate planes
	for(uint8 i = 0; i < this->pixelFormat.nPlanes; i++)
	{
		this->planeLines[i] = this->pixelFormat.ComputeNumberOfLines(i, this->size.height);
		this->lineSizes[i] = this->pixelFormat.ComputeLineSize(i, this->size.width);
		this->planes[i] = MemAlloc(this->lineSizes[i] * this->planeLines[i]);
	}
}

/*DEPRECATED:
//Local functions
static Pixmap *CreateImage(uint16 width, uint16 height, ColorSpace colorSpace)
{
	switch(colorSpace)
	{
		case ColorSpace::RGB:
			return new R8G8B8Image(width, height);
		case ColorSpace::YCbCr:
			return new YCbCr420Image(width, height, true);
	}

	return nullptr;
}

static void ConvertColor(uint16 x, uint16 y, const Pixmap &refSourceImage, Pixmap &refTargetImage)
{
	switch(refSourceImage.GetColorSpace())
	{
		case ColorSpace::RGB:
		{
			const RGBPixmap &srcPixmap = (const RGBPixmap &) refSourceImage;

			switch(refTargetImage.GetColorSpace())
			{
				case ColorSpace::YCbCr:
				{
					Vector3<uint8> c = srcPixmap.GetPixel({x, y});
					uint32 index = y * srcPixmap.GetWidth() + x;

					YCbCr420Image &targetPixmap = dynamic_cast<YCbCr420Image &>(refTargetImage);
					targetPixmap.GetLumaChannel()[index] = static_cast<uint8>(Clamp(16 + (65.738 * c[0]/256 + 129.057 * c[1]/256 + 25.064 * c[2]/256), 0.0, (float64)Natural<uint8>::Max()));

					x /= 2;
					y /= 2;
					index = y * srcPixmap.GetWidth()/2 + x;
					targetPixmap.GetChromaBlueChannel()[index] = static_cast<uint8>(Clamp(128 + (-37.945 * c[0]/256 + -74.494 * c[1]/256 + 112.439 * c[2]/256), 0.0, (float64)Natural<uint8>::Max()));
					targetPixmap.GetChromaRedChannel()[index] = static_cast<uint8>(Clamp(128 + (112.439 * c[0]/256 + -94.154 * c[1]/256 + -18.285 * c[2]/256), 0.0, (float64)Natural<uint8>::Max()));
				}
				break;
				default:
					NOT_IMPLEMENTED_ERROR;
			}
		}
		break;
		case ColorSpace::RGBA:
		{
			const RGBAImage &refSrcImage = (const RGBAImage &)refSourceImage;

			switch(refTargetImage.GetColorSpace())
			{
				case ColorSpace::RGB:
				{
					byte r, g, b, a;

					R8G8B8Image &refDestImage = (R8G8B8Image &)refTargetImage;

					uint32 i = y * refSrcImage.GetWidth() + x;
					refSrcImage.GetPixel(i, r, g, b, a);

					//TODO: verify what is the best choice here
					/*
					//use black as "background"
					refDestImage.GetRedChannel()[i] = byte((a * r) / 255);
					refDestImage.GetGreenChannel()[i] = byte((a * g) / 255);
					refDestImage.GetBlueChannel()[i] = byte((a * b) / 255);
					*//*
					//discard alpha channel
					refDestImage.GetRedChannel()[i] = r;
					refDestImage.GetGreenChannel()[i] = g;
					refDestImage.GetBlueChannel()[i] = b;
				}
					break;
				default:
					NOT_IMPLEMENTED_ERROR;
			}
		}
			break;
		case ColorSpace::YCbCr:
		{
			const YCbCrImage &refSrcImage = (const YCbCrImage &)refSourceImage;

			switch(refTargetImage.GetColorSpace())
			{
				case ColorSpace::RGB:
				{
					byte Y, Cb, Cr;
					int32 value;
					float64 tmp;

					R8G8B8Image &refDestImage = (R8G8B8Image &)refTargetImage;

					uint32 i = y * refSrcImage.GetWidth() + x;
					refSrcImage.GetPixel(i, Y, Cb, Cr);

					tmp = 255.0 / 219.0 * (Y - 16);

					value = int32(tmp + 255.0 / 112.0 * 0.701 * (((int32)Cr) - 128));
					refDestImage.GetRedChannel()[i] = Clamp(value, 0, 0xFF);

					value = int32(tmp - 255.0 / 112.0 * 0.886 * 0.114 / 0.587 * (((int32)Cb) - 128) - 255.0 / 112.0 * 0.701 * 0.299 / 0.587 * (((int32)Cr) - 128));
					refDestImage.GetGreenChannel()[i] = Clamp(value, 0, 0xFF);

					value = int32(tmp + 255.0 / 112.0 * 0.886 * (((int32)Cb) - 128));
					refDestImage.GetBlueChannel()[i] = Clamp(value, 0, 0xFF);
				}
					break;
				default:
					NOT_IMPLEMENTED_ERROR;
			}
		}
			break;
		default:
			NOT_IMPLEMENTED_ERROR;
	}
}

//Public methods
Pixmap *Pixmap::Resample(ColorSpace desiredColorSpace) const
{
	uint32 i;
	Pixmap *pResampledImage;

	pResampledImage = CreateImage(this->width, this->height, desiredColorSpace);

	for(uint16 y = 0; y < this->height; y++)
	{
		for(uint16 x = 0; x < this->width; x++)
		{
			ConvertColor(x, y, *this, *pResampledImage);
		}
	}

	return pResampledImage;
}*/
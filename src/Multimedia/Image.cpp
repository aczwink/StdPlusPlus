/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include <ACStdLib/Multimedia/Image.hpp>
//Local
#include <ACStdLib/Debug.h>
#include <ACStdLib/Multimedia/Images/RGB/R8G8B8Image.hpp>
#include <ACStdLib/Multimedia/Images/RGBAImage.hpp>
#include <ACStdLib/Multimedia/Images/YCbCrImage.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

//Local functions
static Image *CreateImage(uint16 width, uint16 height, ColorSpace colorSpace)
{
	switch(colorSpace)
	{
		case ColorSpace::RGB:
			return new R8G8B8Image(width, height);
	}

	return nullptr;
}

static void ConvertColor(uint32 i, const Image &refSourceImage, Image &refTargetImage)
{
	switch(refSourceImage.GetColorSpace())
	{
		case ColorSpace::RGBA:
		{
			const RGBAImage &refSrcImage = (const RGBAImage &)refSourceImage;

			switch(refTargetImage.GetColorSpace())
			{
				case ColorSpace::RGB:
				{
					byte r, g, b, a;

					R8G8B8Image &refDestImage = (R8G8B8Image &)refTargetImage;

					refSrcImage.GetPixel(i, r, g, b, a);

					//TODO: verify what is the best choice here
					/*
					//use black as "background"
					refDestImage.GetRedChannel()[i] = byte((a * r) / 255);
					refDestImage.GetGreenChannel()[i] = byte((a * g) / 255);
					refDestImage.GetBlueChannel()[i] = byte((a * b) / 255);
					*/
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

					refSrcImage.GetPixel(i, Y, Cb, Cr);

					tmp = 255.0 / 219.0 * (Y - 16);

					value = int32(tmp + 255.0 / 112.0 * 0.701 * (((int32)Cr) - 128));
					refDestImage.GetRedChannel()[i] = CLAMP(value, 0, 0xFF);

					value = int32(tmp - 255.0 / 112.0 * 0.886 * 0.114 / 0.587 * (((int32)Cb) - 128) - 255.0 / 112.0 * 0.701 * 0.299 / 0.587 * (((int32)Cr) - 128));
					refDestImage.GetGreenChannel()[i] = CLAMP(value, 0, 0xFF);

					value = int32(tmp + 255.0 / 112.0 * 0.886 * (((int32)Cb) - 128));
					refDestImage.GetBlueChannel()[i] = CLAMP(value, 0, 0xFF);
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
Image *Image::Resample(ColorSpace desiredColorSpace) const
{
	uint32 i;
	Image *pResampledImage;

	pResampledImage = CreateImage(this->width, this->height, desiredColorSpace);

	for(i = 0; i < this->GetNumberOfPixels(); i++)
	{
		ConvertColor(i, *this, *pResampledImage);
	}

	return pResampledImage;
}
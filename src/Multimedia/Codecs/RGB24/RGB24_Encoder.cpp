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
#include "RGB24_Encoder.hpp"
//Local
#include <ACStdLib/Multimedia/Images/RGBImage.hpp>
#include <ACStdLib/Multimedia/Frame.hpp>
#include <ACStdLib/Debug.h>
#include <ACStdLib/Multimedia/VideoFrame.hpp>
#include <ACStdLib/Multimedia/Packet.hpp>

//Public methods
void RGB24_Encoder::Encode(const Frame &frame, Packet &packet) const
{
	bool deleteRGBImage;
	byte r, g, b;
	byte *pRGB;
	Image *pImage;
	RGBImage *pRGBImage;

	ASSERT(frame.GetType() == DataType::Video);

	VideoFrame &refVideoFrame = (VideoFrame &)frame;

	pImage = refVideoFrame.GetImage();

	//convert to RGB if neccessary
	if(pImage->GetColorSpace() == ColorSpace::RGB)
	{
		pRGBImage = (RGBImage *)pImage;
		deleteRGBImage = false;
	}
	else
	{
		pRGBImage = (RGBImage *)pImage->Resample(ColorSpace::RGB);
		deleteRGBImage = true;
	}

	//fill out packet
	packet.Allocate(3 * pRGBImage->GetNumberOfPixels());
	packet.pts = frame.pts;

	//fill data
	pRGB = packet.GetData();
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
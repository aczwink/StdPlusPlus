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
#include "BGR24_Encoder.hpp"
//Local
#include <ACStdLib/Multimedia/Images/RGBImage.hpp>
#include <ACStdLib/Multimedia/Packet.hpp>
#include <ACStdLib/Multimedia/VideoFrame.hpp>

//Public methods
void BGR24_Encoder::Encode(const Frame &refFrame, Packet &packet) const
{
	byte r, g, b;
	byte *pBGR;
	RGBImage *pRGBImage;

	VideoFrame &refVideoFrame = (VideoFrame &)refFrame;

	//get image
	if(refVideoFrame.GetImage()->GetColorSpace() == ColorSpace::RGB)
	{
		pRGBImage = (RGBImage *)refVideoFrame.GetImage();
	}
	else
	{
		pRGBImage = (RGBImage *)refVideoFrame.GetImage()->Resample(ColorSpace::RGB);
	}

	//encode
	packet.Allocate(3 * pRGBImage->GetNumberOfPixels());

	pBGR = packet.GetData();
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
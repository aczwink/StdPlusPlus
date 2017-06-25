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
	byte r, g, b;
	byte *rgb;
	RGBImage *rgbImage;

	ASSERT(frame.GetType() == DataType::Video);

	VideoFrame &videoFrame = (VideoFrame &)frame;
	//get image
	if(videoFrame.GetImage()->GetColorSpace() == ColorSpace::RGB)
	{
		rgbImage = (RGBImage *)videoFrame.GetImage();
	}
	else
	{
		rgbImage = (RGBImage *)videoFrame.GetImage()->Resample(ColorSpace::RGB);
	}

	//fill out packet
	packet.Allocate(3 * rgbImage->GetNumberOfPixels());
	packet.pts = frame.pts;

	//fill data
	rgb = packet.GetData();
	for(uint32 i = 0; i < rgbImage->GetNumberOfPixels(); i++)
	{
		rgbImage->GetPixel(i, r, g, b);

		*rgb++ = r;
		*rgb++ = g;
		*rgb++ = b;
	}

	//clean up
	if(videoFrame.GetImage() != rgbImage)
	{
		delete rgbImage; //the frame that we converted
	}
}
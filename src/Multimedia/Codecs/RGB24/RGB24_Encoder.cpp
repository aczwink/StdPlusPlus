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
#include "RGB24_Encoder.hpp"
//Local
#include <Std++/Multimedia/Pixmaps/RGBPixmap.hpp>
#include <Std++/Multimedia/Frame.hpp>
#include <Std++/Debug.hpp>
#include <Std++/Multimedia/VideoFrame.hpp>
#include <Std++/Multimedia/Packet.hpp>

//Public methods
void RGB24_Encoder::Encode(const Frame &frame)
{
	byte *rgb;
	RGBPixmap *rgbImage;

	ASSERT(frame.GetType() == DataType::Video, u8"If you see this, report to StdXX");

	VideoFrame &videoFrame = (VideoFrame &)frame;
	//get image
	if(videoFrame.GetImage()->GetColorSpace() == ColorSpace::RGB)
	{
		rgbImage = (RGBPixmap *)videoFrame.GetImage();
	}
	else
	{
		rgbImage = (RGBPixmap *)videoFrame.GetImage()->Resample(ColorSpace::RGB);
	}

	//fill out packet
	Packet *packet = new Packet;
	packet->Allocate(3 * rgbImage->GetNumberOfPixels());
	packet->pts = frame.pts;

	//fill data
	rgb = packet->GetData();
	for(uint16 y = 0; y < rgbImage->GetHeight(); y++)
	{
		for(uint16 x = 0; x < rgbImage->GetWidth(); x++)
		{
			auto color = rgbImage->GetPixel(Math::Point<uint16>(y, x));

			*rgb++ = color[0];
			*rgb++ = color[1];
			*rgb++ = color[2];
		}
	}
	this->AddPacket(packet);

	//clean up
	if(videoFrame.GetImage() != rgbImage)
	{
		delete rgbImage; //the frame that we converted
	}
}

void RGB24_Encoder::Flush()
{
	//this encoder always writes through
}
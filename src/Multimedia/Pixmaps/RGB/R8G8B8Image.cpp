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
 /*DEPRECATED:
//Class header
#include <Std++/Multimedia/Pixmaps/RGB/R8G8B8Image.hpp>
//Local
#include <Std++/Memory.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::Multimedia;

//Constructor
R8G8B8Image::R8G8B8Image(uint16 width, uint16 height) : RGBPixmap(width, height)
{
	this->pBlueChannel = (byte *)MemAlloc(width * height);
	this->pGreenChannel = (byte *)MemAlloc(width * height);
	this->pRedChannel = (byte *)MemAlloc(width * height);
}

//Destructor
R8G8B8Image::~R8G8B8Image()
{
	MemFree(this->pBlueChannel);
	MemFree(this->pGreenChannel);
	MemFree(this->pRedChannel);
}

//Public methods
void R8G8B8Image::GetInterpolatedPixel(float32 x, float32 y, byte &refR, byte &refG, byte &refB) const
{
	byte r[4], g[4], b[4];
	int32 fx, fy, w[4], tmpR, tmpG, tmpB;
	float32 p[4];

	//floor
	fx = (int32)x;
	fy = (int32)y;

	//get neighbor pixels
	this->GetPixel(fx, fy, r[0], g[0], b[0]);
	this->GetPixel(fx + 1, fy, r[1], g[1], b[1]);
	this->GetPixel(fx, fy + 1, r[2], g[2], b[2]);
	this->GetPixel(fx + 1, fy + 1, r[3], g[3], b[3]);

	//calc weights
	p[0] = x - fx;
	p[1] = y - fy;
	p[2] = 1.0f - p[0];
	p[3] = 1.0f - p[1];

	w[0] = int32(p[2] * p[3] * 256);
	w[1] = int32(p[0] * p[3] * 256);
	w[2] = int32(p[2] * p[1] * 256);
	w[3] = int32(p[0] * p[1] * 256);

	tmpR = r[0] * w[0] + r[1] * w[1] + r[2] * w[2] + r[3] * w[3];
	tmpG = g[0] * w[0] + g[1] * w[1] + g[2] * w[2] + g[3] * w[3];
	tmpB = b[0] * w[0] + b[1] * w[1] + b[2] * w[2] + b[3] * w[3];

	refR = tmpR >> 8;
	refG = tmpG >> 8;
	refB = tmpB >> 8;
}

Vector3<uint8> R8G8B8Image::GetPixel(const Point<uint16> &p) const
{
	uint32 index = p.y * this->GetWidth() + p.x;
	return Vector3<uint8>(this->pRedChannel[index], this->pGreenChannel[index], this->pBlueChannel[index]);
}

Pixmap *R8G8B8Image::Resample(uint16 desiredWidth, uint16 desiredHeight, ColorSpace desiredColorSpace) const
{
	ASSERT(this->GetColorSpace() == ColorSpace::RGB && desiredColorSpace == ColorSpace::RGB, u8"TODO: Implement this right");
	R8G8B8Image *resampled = new R8G8B8Image(desiredWidth, desiredHeight);
	//Pixmap *resampled = CreateImage(desiredWidth, desiredHeight, desiredColorSpace);

	float64 scaleX = (this->GetWidth()) / (float64)desiredWidth;
	float64 scaleY = (this->GetHeight()) / (float64)desiredHeight;

	for(uint16 y = 0; y < desiredHeight; y++)
	{
		for(uint16 x = 0; x < desiredWidth; x++)
		{
			byte r, g, b;
			this->GetInterpolatedPixel(scaleX * x, scaleY * y, r, g, b);

			resampled->SetPixel(y * desiredWidth + x, r, g, b);
		}
	}

	return resampled;
}

void R8G8B8Image::SetPixel(uint32 index, byte r, byte g, byte b)
{
	this->pBlueChannel[index] = b;
	this->pGreenChannel[index] = g;
	this->pRedChannel[index] = r;
}*/
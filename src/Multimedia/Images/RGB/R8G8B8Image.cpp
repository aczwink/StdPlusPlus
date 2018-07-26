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
#include <Std++/Multimedia/Images/RGB/R8G8B8Image.hpp>
//Local
#include <Std++/Memory.h>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructor
R8G8B8Image::R8G8B8Image(uint16 width, uint16 height) : RGBImage(width, height)
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
void R8G8B8Image::GetPixel(uint32 index, byte &refR, byte &refG, byte &refB) const
{
	refB = this->pBlueChannel[index];
	refG = this->pGreenChannel[index];
	refR = this->pRedChannel[index];
}

void R8G8B8Image::SetPixel(uint32 index, byte r, byte g, byte b)
{
	this->pBlueChannel[index] = b;
	this->pGreenChannel[index] = g;
	this->pRedChannel[index] = r;
}
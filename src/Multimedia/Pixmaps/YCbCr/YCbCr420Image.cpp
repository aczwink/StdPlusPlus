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
 /*DEPRECATED
//Class header
#include <Std++/Multimedia/Pixmaps/YCbCr/YCbCr420Image.hpp>
//Local
#include <Std++/Memory.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructors
YCbCr420Image::YCbCr420Image(uint16 width, uint16 height, bool useFullRange) : YCbCrImage(width, height, useFullRange)
{
	this->pY = (uint8 *)MemAlloc(this->GetNumberOfPixels());
	this->pCb = (uint8 *)MemAlloc(this->GetNumberOfChromaSamples());
	this->pCr = (uint8 *)MemAlloc(this->GetNumberOfChromaSamples());
}

YCbCr420Image::YCbCr420Image(const YCbCr420Image &refSrc) : YCbCrImage(refSrc.GetWidth(), refSrc.GetHeight(), refSrc.UsesFullRange())
{
	this->pY = (uint8 *)MemAlloc(this->GetNumberOfPixels());
	this->pCb = (uint8 *)MemAlloc(this->GetNumberOfChromaSamples());
	this->pCr = (uint8 *)MemAlloc(this->GetNumberOfChromaSamples());

	MemCopy(this->pY, refSrc.pY, this->GetNumberOfPixels());
	MemCopy(this->pCb, refSrc.pCb, this->GetNumberOfChromaSamples());
	MemCopy(this->pCr, refSrc.pCr, this->GetNumberOfChromaSamples());
}

//Destructor
YCbCr420Image::~YCbCr420Image()
{
	MemFree(this->pY);
	MemFree(this->pCb);
	MemFree(this->pCr);
}

//Public methods
void YCbCr420Image::GetPixel(uint32 index, byte &refY, byte &refCb, byte &refCr) const
{
	uint32 row, col, chromaIndex;

	row = index / this->GetWidth();
	col = index % this->GetWidth();
	chromaIndex = (row / 2 * this->GetWidth() / 2) + col / 2;

	refY = this->pY[index];
	refCb = this->pCb[chromaIndex];
	refCr = this->pCr[chromaIndex];
}*/
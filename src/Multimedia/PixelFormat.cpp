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
#include <Std++/Multimedia/PixelFormat.hpp>
//Local
#include <Std++/Mathematics.hpp>
#include <Std++/Containers/Array/FixedArray.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructor
PixelFormat::PixelFormat(NamedPixelFormat namedPixelFormat)
{
	switch(namedPixelFormat)
	{
		case NamedPixelFormat::RGB_24:
			{
				this->colorSpace = ColorSpace::RGB;
				this->nPlanes = 1;

				uint8 shift = 0;
				for(uint8 i = 0; i < 3; i++)
				{
					auto &cc = this->colorComponents[i];

					cc.planeIndex = 0;
					cc.nBits = 8;
					cc.shift = shift;
					cc.isFloat = false;
					cc.min.u8 = 0;
					cc.max.u8 = Natural<uint8>::Max();

					shift += 8;
				}
			}
			break;
		case NamedPixelFormat::YCbCr_420_P:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
}

//Operators
bool PixelFormat::operator==(const PixelFormat &other) const
{
	if(this->colorSpace != other.colorSpace || this->nPlanes != other.nPlanes)
		return false;

	for(uint8 i = 0; i < this->nPlanes; i++)
	{
		if(MemCmp(&this->colorComponents[i], &other.colorComponents[i], sizeof(this->colorComponents[i])))
			return false;
	}

	return true;
}

//Public methods
uint32 PixelFormat::ComputeLineSize(uint8 planeIndex, uint16 nPixelsPerRow) const
{
	uint8 nBits = 0;
	for(uint8 i = 0; i < this->GetNumberOfColorComponents(); i++)
	{
		const auto &cc = this->colorComponents[i];
		if(cc.planeIndex == planeIndex)
			nBits = Math::Max(nBits, uint8(cc.shift + cc.nBits));
	}

	return static_cast<uint32>(((nBits / 8) + (((nBits % 8) == 0) ? 0 : 1 )) * nPixelsPerRow);
}

bool PixelFormat::GetNameIfExisting(NamedPixelFormat &namedPixelFormat) const
{
	FixedArray<NamedPixelFormat> names({NamedPixelFormat::RGB_24, NamedPixelFormat::YCbCr_420_P});

	for(NamedPixelFormat name : names)
	{
		if(*this == PixelFormat(name))
		{
			namedPixelFormat = name;
			return true;
		}
	}

	return false;
}

uint8 PixelFormat::GetNumberOfColorComponents() const
{
	switch(this->colorSpace)
	{
		case ColorSpace::RGB:
		case ColorSpace::YCbCr:
			return 3;
		case ColorSpace::RGBA:
			return 4;
	}
	return 0;
}
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
					cc.horzSampleFactor = 1;
					cc.vertSampleFactor = 1;
					cc.min.u8 = 0;
					cc.max.u8 = Natural<uint8>::Max();

					shift += 8;
				}
			}
			break;
		case NamedPixelFormat::YCbCr_420_P:
		{
			this->colorSpace = ColorSpace::YCbCr;
			this->nPlanes = 3;

			for (uint8 i = 0; i < 3; i++)
			{
				auto &cc = this->colorComponents[i];

				cc.planeIndex = i;
				cc.nBits = 8;
				cc.shift = 0;
				cc.isFloat = false;
				cc.min.u8 = 16; //foot room
			}
			//headroom
			this->colorComponents[0].max.u8 = 235;
			this->colorComponents[1].max.u8 = 240;
			this->colorComponents[2].max.u8 = 240;

			this->colorComponents[0].horzSampleFactor = 1;
			this->colorComponents[0].vertSampleFactor = 1;
			//cb and cr are sub-sampled in both directions
			this->colorComponents[1].horzSampleFactor = 2;
			this->colorComponents[1].vertSampleFactor = 2;
			this->colorComponents[2].horzSampleFactor = 2;
			this->colorComponents[2].vertSampleFactor = 2;
		}
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
	/**
	 * We assume here that there are not different sample factors per plane.
	 * This would lead to having multiple line sizes for one plane.
	 * However, to not cause problems we will use the smallest sample factor (i.e. best sampling)
	 * and thus return the largest line-size (which will cause over-allocations).
	 * Actually I also don't believe that this case exists in practical cases.
	 */
	uint8 nBitsMax = 0;
	uint8 sampleFactor = Natural<uint8>::Max();
	for(uint8 i = 0; i < this->GetNumberOfColorComponents(); i++)
	{
		const auto &cc = this->colorComponents[i];
		if (cc.planeIndex == planeIndex)
		{
			nBitsMax = Math::Max(nBitsMax, uint8(cc.shift + cc.nBits));
			sampleFactor = Math::Min(sampleFactor, cc.horzSampleFactor);
		}
	}

	return static_cast<uint32>(((nBitsMax / 8) + (((nBitsMax % 8) == 0) ? 0 : 1 )) * nPixelsPerRow / sampleFactor);
}

uint32 PixelFormat::ComputeNumberOfLines(uint8 planeIndex, uint16 height) const
{
	//We assume here that there are not different sample factors per plane (see PixelFormat::ComputeLineSize)
	uint32 nLinesMax = 0;
	for (uint8 i = 0; i < this->GetNumberOfColorComponents(); i++)
	{
		const auto &cc = this->colorComponents[i];
		if (cc.planeIndex == planeIndex)
			nLinesMax = Math::Max(nLinesMax, uint32(height / cc.vertSampleFactor));
	}

	return nLinesMax;
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
/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
#pragma once
//Local
#include "../Definitions.h"
#include "EnumTypes.hpp"

namespace StdXX
{
    namespace Multimedia
    {
		enum class NamedPixelFormat
		{
			BGR_24, //packed uint8 RGB ordered B G R
			RGB_24, //packed uint8 RGB
			YCbCr_420_P //planar uint8 YCbCr, Cb & Cr are subsampled by factor of 2 horizontally and vertically
		};

        class STDPLUSPLUS_API PixelFormat
        {
		public:
        	//Constants
        	static const uint8 MAX_COLOR_COMPONENTS = 4;
        	static const uint8 MAX_PLANES = 3;

        	//Members
        	ColorSpace colorSpace;
        	uint8 nPlanes;
        	struct
			{
				/**
				 * In which plane is the color component found.
				 */
				uint8 planeIndex;
				/**
				 * How many bits does the color component take.
				 */
				uint8 nBits;
				/**
				 * Specifies the number of bits that are discarded, before 'nBits' are read.
				 */
				uint8 shift;
				/**
				 * Should the color component be interpreted as float or integer.
				 */
				bool isFloat;
				/**
				 * Minimum value the component can take.
				 */
				union
				{
					uint8 u8;
				} min;
				/**
				 * Maximum value the component can take.
				 */
				union
				{
					uint8 u8;
				} max;
			} colorComponents[MAX_COLOR_COMPONENTS];
			struct
			{
				/**
				 * We assume here that there are not different sample factors per plane.
				 * This would lead to having multiple line sizes for one plane.
				 * Actually I also don't believe that this case exists in practical cases.
				 * However, to not cause problems we will use the smallest sample factor (i.e. best sampling)
				 * and thus use the largest line-size (which will cause over-allocations).
				 */
				/**
				 * The sampling factor in horizontal direction.
				 * 1 if one sample is used for 1 pixel.
				 * 2 if one sample is used for 2 (horizontally) consecutive pixels.
				 * etc.
				 */
				uint8 horzSampleFactor;
				/**
				* The sampling factor in vertical direction.
				* 1 if one sample is used for 1 pixel.
				* 2 if one sample is used for 2 (vertically) consecutive pixels.
				* etc.
				*/
				uint8 vertSampleFactor;
			} planes[MAX_PLANES];

        	//Constructors
			inline PixelFormat(ColorSpace colorSpace) : colorSpace(colorSpace)
			{
			}

			PixelFormat(NamedPixelFormat namedPixelFormat);

			//Operators
			bool operator==(const PixelFormat &other) const;

			inline bool operator!=(const PixelFormat &other) const
			{
				return !(*this == other);
			}

        	//Methods
			uint8 ComputeBlockSize(uint8 planeIndex) const;
			uint32 ComputeNumberOfLines(uint8 planeIndex, uint16 height) const;
			bool GetNameIfExisting(NamedPixelFormat &namedPixelFormat) const;
			uint8 GetNumberOfColorComponents() const;

			//Inline
			inline uint32 ComputeLineSize(uint8 planeIndex, uint16 nPixelsPerRow) const
			{
				return this->ComputeBlockSize(planeIndex) * nPixelsPerRow / this->planes[planeIndex].horzSampleFactor;
			}
        };
    }
}
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
#pragma once
//Local
#include <Std++/Math/Size.hpp>
#include "../Definitions.h"
#include "EnumTypes.hpp"
#include "PixelFormat.hpp"

namespace StdXX
{
    namespace Multimedia
    {
        class STDPLUSPLUS_API Pixmap
        {
        public:
            //Constructor
        	inline Pixmap(const Math::Size<uint16> &size, const PixelFormat &pixelFormat)
					: size(size), pixelFormat(pixelFormat)
			{
				this->Allocate();
			}

			inline Pixmap(const Math::Size<uint16> &size, NamedPixelFormat namedPixelFormat)
					: size(size), pixelFormat(namedPixelFormat)
			{
				this->Allocate();
			}

            //Destructor
			~Pixmap();

            //Inline
			inline uint32 GetLineSize(uint8 planeIndex) const
			{
				return this->lineSizes[planeIndex];
			}

			inline uint32 GetNumberOfLines(uint8 planeIndex) const
			{
				return this->planeLines[planeIndex];
			}

			inline const PixelFormat &GetPixelFormat() const
			{
				return this->pixelFormat;
			}

			inline void *GetPlane(uint8 i)
			{
				return this->planes[i];
			}

			inline const void *GetPlane(uint8 i) const
			{
				return this->planes[i];
			}

			inline const Math::Size<uint16> &GetSize() const
			{
				return this->size;
			}

            inline uint32 GetNumberOfPixels() const
            {
                return (uint32)this->size.width * (uint32)this->size.height;
            }

        private:
        	//Members
        	Math::Size<uint16> size;
            PixelFormat pixelFormat;
            void *planes[PixelFormat::MAX_PLANES];
			uint32 planeLines[PixelFormat::MAX_PLANES];
            uint32 lineSizes[PixelFormat::MAX_PLANES];

            //Methods
			void Allocate();
        };
    }
}
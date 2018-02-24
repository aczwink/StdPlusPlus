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
#include "../Definitions.h"
#include "EnumTypes.hpp"

namespace StdPlusPlus
{
    namespace Multimedia
    {
        class STDPLUSPLUS_API Image
        {
        private:
            //Members
            uint16 width;
            uint16 height;

        public:
            //Constructor
            inline Image(uint16 width, uint16 height)
            {
                this->width = width;
                this->height = height;
            }

            //Destructor
            virtual ~Image(){}

            //Abstract
            virtual ColorSpace GetColorSpace() const = 0;

            //Methods
            Image *Resample(ColorSpace desiredColorSpace) const;
            Image *Resample(uint16 desiredWidth, uint16 desiredHeight, ColorSpace desiredColorSpace) const;

            //Inline
            inline uint16 GetHeight() const
            {
                return this->height;
            }

            inline uint16 GetWidth() const
            {
                return this->width;
            }

            inline uint32 GetNumberOfPixels() const
            {
                return (uint32)this->width * (uint32)this->height;
            }
        };
    }
}
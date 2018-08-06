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
#include "Std++/Multimedia/Pixmap.hpp"

namespace StdXX
{
    namespace Multimedia
    {
        /*
        Y = Luminance from black to white
        Cb = Chrominance blue from grey to blue
        Cr = Chrominance red from grey to red
        */
        class STDPLUSPLUS_API YCbCrImage : public Pixmap
        {
        private:
            //Members
            /*
            values usually don't use full 8-bit range, see this: http://www.equasys.de/colorconversion.html
            the limited ranges are the following:
            Luminance in range [16, 235]
            Chrominance in range [16, 240]
            */
            bool useFullRange;
        public:
            //Constructor
            inline YCbCrImage(uint16 width, uint16 height, bool useFullRange) : Pixmap(width, height)
            {
                this->useFullRange = useFullRange;
            }

            //Abstract
            virtual void GetPixel(uint32 index, byte &refY, byte &refCb, byte &refCr) const = 0;

            //Methods
            ColorSpace GetColorSpace() const;

            //Inline
            inline bool UsesFullRange() const
            {
                return this->useFullRange;
            }
        };
    }
}
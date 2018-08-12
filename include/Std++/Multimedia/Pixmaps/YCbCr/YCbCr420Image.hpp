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
#pragma once
//Local
#include "../YCbCrImage.hpp"

namespace StdXX
{
    namespace Multimedia
    {
        //YCbCr 4:2:0 has 1 Cb, and 1 Cr value for 4 Y values
        class STDPLUSPLUS_API YCbCr420Image : public YCbCrImage
        {
        private:
            //Members
            uint8 *pY;
            uint8 *pCb;
            uint8 *pCr;

            //Inline
            inline uint32 GetNumberOfChromaSamples() const
            {
                return this->GetNumberOfPixels() / 4;
            }

        public:
            //Constructors
            YCbCr420Image(uint16 width, uint16 height, bool useFullRange);
            YCbCr420Image(const YCbCr420Image &refSrc);

            //Destructor
            ~YCbCr420Image();

            //Methods
            void GetPixel(uint32 index, byte &refY, byte &refCb, byte &refCr) const;

            //Inline
            inline uint8 *GetChromaBlueChannel()
            {
                return this->pCb;
            }

            inline uint8 *GetChromaRedChannel()
            {
                return this->pCr;
            }

            inline uint8 *GetLumaChannel()
            {
                return this->pY;
            }
        };
    }
}*/
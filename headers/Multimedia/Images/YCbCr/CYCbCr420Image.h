/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
//Local
#include "../AYCbCrImage.h"

namespace ACStdLib
{
    namespace Multimedia
    {
        //YCbCr 4:2:0 has 1 Cb, and 1 Cr value for 4 Y values
        class ACSTDLIB_API CYCbCr420Image : public AYCbCrImage
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
            CYCbCr420Image(uint16 width, uint16 height, bool useFullRange);
            CYCbCr420Image(const CYCbCr420Image &refSrc);

            //Destructor
            ~CYCbCr420Image();

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
}
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
#include "../ARGBImage.h"

namespace ACStdLib
{
    namespace Multimedia
    {
        class ACSTDLIB_API CR8G8B8Image : public ARGBImage
        {
        private:
            //Members
            byte *pRedChannel;
            byte *pGreenChannel;
            byte *pBlueChannel;
        public:
            //Constructor
            CR8G8B8Image(uint16 width, uint16 height);

            //Destructor
            ~CR8G8B8Image();

            //Derived methods
            using ARGBImage::SetPixel;

            //Methods
            void GetPixel(uint32 index, byte &refR, byte &refG, byte &refB) const;
            void SetPixel(uint32 index, byte r, byte g, byte b);

            //Inline
            inline byte *GetBlueChannel()
            {
                return this->pBlueChannel;
            };

            inline byte *GetGreenChannel()
            {
                return this->pGreenChannel;
            };

            inline byte *GetRedChannel()
            {
                return this->pRedChannel;
            };
        };
    }
}
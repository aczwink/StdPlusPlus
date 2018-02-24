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
#include "../RGBAImage.hpp"

namespace StdPlusPlus
{
    namespace Multimedia
    {
        class STDPLUSPLUS_API CR8G8B8A8Image : public RGBAImage
        {
        private:
            //Members
            byte *pRedChannel;
            byte *pGreenChannel;
            byte *pBlueChannel;
            byte *pAlphaChannel;
        public:
            //Constructor
            CR8G8B8A8Image(uint16 width, uint16 height);

            //Destructor
            ~CR8G8B8A8Image();

            //Methods
            void GetPixel(uint32 index, byte &refR, byte &refG, byte &refB, byte &refA) const;

            //Inline
            inline byte *GetAlphaChannel()
            {
                return this->pAlphaChannel;
            }

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
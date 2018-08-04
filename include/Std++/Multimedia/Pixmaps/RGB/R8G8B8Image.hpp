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
#include "Std++/Multimedia/Pixmaps/RGBPixmap.hpp"

namespace StdXX
{
    namespace Multimedia
    {
        class STDPLUSPLUS_API R8G8B8Image : public RGBPixmap
        {
        private:
            //Members
            byte *pRedChannel;
            byte *pGreenChannel;
            byte *pBlueChannel;
        public:
            //Constructor
            R8G8B8Image(uint16 width, uint16 height);

            //Destructor
            ~R8G8B8Image();

            //Methods
            void GetInterpolatedPixel(float32 x, float32 y, byte &refR, byte &refG, byte &refB) const;
			Math::Vector3<uint8> GetPixel(const Math::Point<uint16> &p) const override;
			Pixmap *Resample(uint16 desiredWidth, uint16 desiredHeight, ColorSpace desiredColorSpace) const;
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
            }

			inline void GetPixel(int32 x, int32 y, byte &r, byte &g, byte &b) const
			{
				uint32 index = y * this->GetWidth() + x;
				r = this->pRedChannel[index];
				g = this->pGreenChannel[index];
				b = this->pBlueChannel[index];
			}
		};
    }
}
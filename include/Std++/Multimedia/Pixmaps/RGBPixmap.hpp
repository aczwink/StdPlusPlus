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
#include <Std++/Math/Point.hpp>
#include <Std++/Multimedia/Pixmap.hpp>
#include <Std++/Math/Vector3.hpp>

namespace StdXX
{
    namespace Multimedia
    {
        class STDPLUSPLUS_API RGBPixmap : public Pixmap
        {
        public:
            //Constructor
            inline RGBPixmap(uint16 width, uint16 height) : Pixmap(width, height)
            {
            }

            //Abstract
            virtual Math::Vector3<uint8> GetPixel(const Math::Point<uint16> &p) const = 0;

            //Methods
            ColorSpace GetColorSpace() const;
        };
    }
}
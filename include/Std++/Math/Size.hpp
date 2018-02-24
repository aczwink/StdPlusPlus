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

namespace StdPlusPlus
{
    class STDPLUSPLUS_API Size
    {
    public:
        //Members
        uint16 width;
        uint16 height;

        //Constructors
        inline Size()
        {
            this->width = 0;
            this->height = 0;
        }

        inline Size(uint16 width, uint16 height)
        {
            this->width = width;
            this->height = height;
        }

        //Inline operators
        inline Size operator+(const Size &refRight) const
        {
            return Size(this->width + refRight.width, this->height + refRight.height);
        }

        inline Size &operator+=(const Size &refRight)
        {
            this->width += refRight.width;
            this->height += refRight.height;

            return *this;
        }

        inline Size operator*(uint16 i) const
        {
            return Size(this->width * i, this->height * i);
        }

        inline Size operator/(uint16 i) const
        {
            return Size(this->width / i, this->height / i);
        }

        //Functions
        static Size GetScreenSize();
    };
}
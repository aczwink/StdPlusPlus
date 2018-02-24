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
    class STDPLUSPLUS_API Point
    {
    public:
        //Members
        int32 x;
        int32 y;

        //Constructors
        inline Point()
        {
            this->x = 0;
            this->y = 0;
        }

        inline Point(int32 x, int32 y)
        {
            this->x = x;
            this->y = y;
        }

        //Inline operators
        inline Point operator+(const Point &refRight) const
        {
            return Point(this->x + refRight.x, this->y + refRight.y);
        }

        inline Point operator-(const Point &right) const
        {
            return Point(this->x - right.x, this->y - right.y);
        }
    };
}
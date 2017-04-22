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
#include "Definitions.h"

namespace ACStdLib
{
    class ACSTDLIB_API CColor
    {
    public:
        //Members
        float32 r;
        float32 g;
        float32 b;
        float32 a;

        //Constructors
        inline CColor()
        {
            this->r = 0;
            this->g = 0;
            this->b = 0;
            this->a = 0;
        }

        inline CColor(float32 r, float32 g, float32 b, float32 a)
        {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }
    };
}
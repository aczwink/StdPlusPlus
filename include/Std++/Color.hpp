/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
#include "Definitions.h"

namespace StdXX
{
    class STDPLUSPLUS_API Color
    {
    public:
        //Members
        float32 r;
        float32 g;
        float32 b;
        float32 a;

        //Constructors
        inline Color()
        {
            this->r = 0;
            this->g = 0;
            this->b = 0;
            this->a = 1;
        }

		inline Color(float32 r, float32 g, float32 b)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = 1;
		}

        inline Color(float32 r, float32 g, float32 b, float32 a)
        {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }
    };
}
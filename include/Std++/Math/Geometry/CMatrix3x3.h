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
#include "../../Definitions.h"
#include "../Vector3.hpp"

namespace StdPlusPlus
{
    namespace Math
    {
        //Forward declarations
        class Matrix4x4;

        class STDPLUSPLUS_API CMatrix3x3
        {
        private:
            //Members
            Vector3 columns[4];

        public:
            //Constructors
            inline CMatrix3x3()
            {
            }

            CMatrix3x3(const Matrix4x4 &refMat);

            //Operators
            inline float32 &operator()(uint8 x, uint8 y)
            {
                ASSERT(x < 3, "Column must be < 3");

                return this->columns[x][y];
            }

            inline const float32 &operator()(uint8 x, uint8 y) const
            {
                ASSERT(x < 3, "Column must be < 3");

                return this->columns[x][y];
            }

            inline Vector3 &operator[](uint8 x)
            {
                ASSERT(x < 3, "Column must be < 3");

                return this->columns[x];
            }

            inline const Vector3 &operator[](uint8 x) const
            {
                ASSERT(x < 3, "Column must be < 3");

                return this->columns[x];
            }
        };
    }
}
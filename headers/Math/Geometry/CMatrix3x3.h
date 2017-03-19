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
#include "../../Definitions.h"
#include "CVector3.h"

namespace ACStdLib
{
    namespace Math
    {
        //Forward declarations
        class CMatrix4x4;

        class ACSTDLIB_API CMatrix3x3
        {
        private:
            //Members
            CVector3 columns[4];

        public:
            //Constructors
            inline CMatrix3x3()
            {
            }

            CMatrix3x3(const CMatrix4x4 &refMat);

            //Operators
            inline float32 &operator()(uint8 x, uint8 y)
            {
                ASSERT(x < 3);

                return this->columns[x][y];
            }

            inline const float32 &operator()(uint8 x, uint8 y) const
            {
                ASSERT(x < 3);

                return this->columns[x][y];
            }

            inline CVector3 &operator[](uint8 x)
            {
                ASSERT(x < 3);

                return this->columns[x];
            }

            inline const CVector3 &operator[](uint8 x) const
            {
                ASSERT(x < 3);

                return this->columns[x];
            }
        };
    }
}
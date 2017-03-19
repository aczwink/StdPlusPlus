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
#include "CVector2.h"

namespace ACStdLib
{
    namespace Math
    {
        class ACSTDLIB_API CMatrix2x2
        {
        private:
            //Members
            CVector2 columns[2];

        public:
            //Operators
            CMatrix2x2 operator*(const CMatrix2x2 &refRight) const;

            //Inline operators
            inline float32 &operator()(uint8 x, uint8 y)
            {
                ASSERT(x < 2);

                return this->columns[x][y];
            }

            inline const float32 &operator()(uint8 x, uint8 y) const
            {
                ASSERT(x < 2);

                return this->columns[x][y];
            }

            inline CVector2 &operator[](uint8 x)
            {
                ASSERT(x < 2);

                return this->columns[x];
            }

            //Functions
            static CMatrix2x2 Identity();
            static CMatrix2x2 Scale(float32 scaleX, float32 scaleY);
        };
    }
}
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
#include "Vector2.hpp"

namespace StdXX
{
    namespace Math
    {
        template <typename ScalarType>
        class STDPLUSPLUS_API Matrix2x2
        {
			typedef Matrix2x2<ScalarType> mat2;
			typedef Vector2<ScalarType> vec2;

        public:
            //Operators
            Matrix2x2 operator*(const Matrix2x2 &refRight) const;
			vec2 operator*(const vec2 &v) const;

            //Inline operators
            inline ScalarType &operator()(uint8 row, uint8 col)
            {
                ASSERT(col < 2, "Column must be < 2");

                return this->columns[col][row];
            }

            inline const float32 &operator()(uint8 row, uint8 col) const
            {
                ASSERT(col < 2, "Column must be < 2");

                return this->columns[col][row];
            }

            inline vec2 &operator[](uint8 col)
            {
                ASSERT(col < 2, "Column must be < 2");

                return this->columns[col];
            }

            //Functions
            static Matrix2x2 Identity();
			static mat2 Rotate(ScalarType angle);
            static Matrix2x2 Scale(float32 scaleX, float32 scaleY);

		private:
			//Members
			vec2 columns[2];
        };

		typedef Matrix2x2<float32> Matrix2s;
    }
}
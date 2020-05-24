/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Math/Vector2/Vector2.hpp>

namespace StdXX::Math
{
	template <typename ScalarType>
	class STDPLUSPLUS_API Matrix2x2
	{
		typedef Matrix2x2<ScalarType> mat2;
		typedef Vector2<ScalarType> vec2;

	public:
		//Constructors
		Matrix2x2() = default;

		inline Matrix2x2(const vec2& col1, const vec2& col2)
		{
			this->columns[0] = col1;
			this->columns[1] = col2;
		}

		//Operators
		Matrix2x2 operator*(const Matrix2x2 &right) const;
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

		//Inline
		inline ScalarType Determinant() const
		{
			/*
			 * [ a b ]
			 * [ c d ]
			 * a*d - b*c
			 */
			return this->columns[0].x * this->columns[1].y - this->columns[1].x * this->columns[0].y;
		}

		//Inline functions
		inline static Matrix2x2 Scale(ScalarType scaleX, ScalarType scaleY)
		{
			return Scale({scaleX, scaleY});
		}

		inline static Matrix2x2 Scale(const Math::Vector2<ScalarType>& scale)
		{
			Matrix2x2<ScalarType> matrix;

			matrix(0, 0) = scale.x;
			matrix(1, 1) = scale.y;

			return matrix;
		}

	private:
		//Members
		vec2 columns[2];
	};

	typedef Matrix2x2<float32> Matrix2S;
	typedef Matrix2x2<float64> Matrix2D;
}
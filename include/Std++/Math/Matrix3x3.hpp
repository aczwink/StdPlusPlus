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
#include <Std++/Math/Radian.hpp>
#include "Vector2/Vector2.inl"
#include "Vector3.hpp"

namespace StdXX::Math
{
	//Forward declarations
	template <typename ScalarType>
	class Matrix4x4;

	template <typename ScalarType>
	class Matrix3x3
	{
		typedef Matrix3x3<ScalarType> mat3;
		typedef Vector2<ScalarType> vec2;
		typedef Vector3<ScalarType> vec3;
	public:
		//Constructors
		inline Matrix3x3()
		{
		}

		inline Matrix3x3(const vec3& col1, const vec3& col2, const vec3& col3)
		{
			this->columns[0] = col1;
			this->columns[1] = col2;
			this->columns[2] = col3;
		}

		Matrix3x3(const Matrix4x4<ScalarType> &refMat);

		//Operators
		mat3 operator*(const mat3 &right) const;
		vec3 operator*(const vec3 &v) const;

		inline vec3 &operator[](uint8 x)
		{
			ASSERT(x < 3, "Column must be < 3");

			return this->columns[x];
		}

		inline const vec3 &operator[](uint8 x) const
		{
			ASSERT(x < 3, "Column must be < 3");

			return this->columns[x];
		}

		inline ScalarType &operator()(uint8 y, uint8 x)
		{
			ASSERT(x < 3, "Column must be < 3");

			return this->columns[x][y];
		}

		inline const ScalarType &operator()(uint8 y, uint8 x) const
		{
			ASSERT(x < 3, "Column must be < 3");

			return this->columns[x][y];
		}

		//Functions
		static mat3 Rotation(const Radian<ScalarType> &angle, const vec3 &axis);

		inline static mat3 Identity()
		{
			mat3 identity;

			identity(0, 0) = 1;
			identity(1, 1) = 1;
			identity(2, 2) = 1;

			return identity;
		}

		inline static mat3 Scale(const vec3& scale)
		{
			mat3 matrix;

			matrix(0, 0) = scale.x;
			matrix(1, 1) = scale.y;
			matrix(2, 2) = scale.z;

			return matrix;
		}

		inline static mat3 Translate(const vec2& translate)
		{
			mat3 matrix = mat3::Identity();
			matrix[2] = vec3(translate, 1);

			return matrix;
		}

		//Inline
		inline ScalarType Determinant() const
		{
			return this->columns[0][0] * this->columns[1][1] * this->columns[2][2]
				   - this->columns[0][0] * this->columns[2][1] * this->columns[1][2]
				   - this->columns[1][0] * this->columns[0][1] * this->columns[2][2]
				   + this->columns[1][0] * this->columns[2][1] * this->columns[0][2]
				   + this->columns[2][0] * this->columns[0][1] * this->columns[1][2]
				   - this->columns[2][0] * this->columns[1][1] * this->columns[0][2]
					;
		}

	private:
		//Members
		vec3 columns[3];
	};

	typedef Matrix3x3<float32> Matrix3S;
}
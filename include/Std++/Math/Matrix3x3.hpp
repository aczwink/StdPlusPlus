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
#include <Std++/Math/Radian.hpp>
#include "Vector3.hpp"

namespace StdXX
{
	namespace Math
	{
		//Move declarations
		template <typename ScalarType>
		class Matrix4x4;

		template <typename ScalarType>
		class Matrix3x3
		{
			typedef Matrix3x3<ScalarType> mat3;
			typedef Vector3<ScalarType> vec3;
		public:
			//Constructors
			inline Matrix3x3()
			{
			}

			Matrix3x3(const Matrix4x4<ScalarType> &refMat);

			//Operators
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

		private:
			//Members
			vec3 columns[3];
		};

		typedef Matrix3x3<float32> Matrix3s;
	}
}
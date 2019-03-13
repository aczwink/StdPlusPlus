/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Definitions.h>
#include "Vector3.hpp"

namespace StdXX
{
    namespace Math
    {
		template<typename ScalarType>
		class Quaternion
		{
		public:
			//Members
			/**
			 * The scalar part.
			 */
			ScalarType scalar;
			/**
			 * The vector part before i.
			 */
			ScalarType vi;
			ScalarType vj;
			ScalarType vk;

			//Constructors
			inline Quaternion(ScalarType scalarPart, const Vector3<ScalarType>& vectorPart)
			{
				this->scalar = scalarPart;
				this->vi = vectorPart.x;
				this->vj = vectorPart.y;
				this->vk = vectorPart.z;
			}

			inline Quaternion(ScalarType scalarPart, ScalarType vi, ScalarType vj, ScalarType vk)
			{
				this->scalar = scalarPart;
				this->vi = vi;
				this->vj = vj;
				this->vk = vk;
			}

			//Operators
			inline Quaternion<ScalarType> operator*(const Quaternion<ScalarType>& rhs) const
			{
				return {
					this->scalar * rhs.scalar - this->vi * rhs.vi - this->vj * rhs.vj - this->vk * rhs.vk,
					this->scalar * rhs.vi + this->vi * rhs.scalar + this->vj * rhs.vk - this->vk * rhs.vj,
					this->scalar * rhs.vj - this->vi * rhs.vk + this->vj * rhs.scalar + this->vk * rhs.vi,
					this->scalar * rhs.vk + this->vi * rhs.vj - this->vj * rhs.vi + this->vk * rhs.scalar
				};
			}

			inline Quaternion<ScalarType> operator/(ScalarType scalar) const
			{
				return { this->scalar / scalar, this->vi / scalar, this->vj / scalar, this->vk / scalar };
			}

			//Inline
			inline Quaternion<ScalarType> Conjugate() const
			{
				return { this->scalar, -this->vi, -this->vj, -this->vk };
			}

			inline Quaternion<ScalarType> Inverse() const
			{
				return this->Conjugate() / this->NormSquared();
			}

			inline Vector3<ScalarType> Rotate(const Vector3<ScalarType>& v)
			{
				Quaternion<ScalarType> tmp = *this * Quaternion<ScalarType>(0, v) * this->Inverse();
				return {tmp.vi, tmp.vj, tmp.vk};
			}

			//Functions
			/**
			 *
			 * @param angle
			 * @param axis - Must be normalized!
			 * @return
			 */
			static Quaternion<ScalarType> Rotation(const Radian<ScalarType>& angle, const Vector3<ScalarType>& axis)
			{
				return {
					cos(angle.value / 2),
					axis * sin(angle.value / 2)
				};
			}

		private:
			//Inline
			inline ScalarType NormSquared() const
			{
				return this->scalar * this->scalar + this->vi * this->vi + this->vj * this->vj + this->vk * this->vk;
			}
		};

		typedef Quaternion<float64> QuaternionD;
    }
}
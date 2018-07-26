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
#include "../Definitions.h"
#include "Radian.hpp"
#include "Vector4.hpp"

namespace StdXX
{
    //Move declarations
    class vec4f32;

    namespace Math
    {
        //Move declarations
        template <typename ScalarType>
        class Matrix3x3;
		template <typename ScalarType>
        class Vector3;
		template <typename ScalarType>
        class Vector4;

		template <typename ScalarType>
        class STDPLUSPLUS_API Matrix4x4
        {
			typedef Vector3<ScalarType> vec3;
			typedef Vector4<ScalarType> vec4;
			typedef Matrix4x4<ScalarType> mat4;
        public:
            //Constructors
            inline Matrix4x4()
            {
            }

            inline Matrix4x4(const vec4 &refCol1, const vec4 &refCol2, const vec4 &refCol3, const vec4 &refCol4)
            {
                this->columns[0] = refCol1;
                this->columns[1] = refCol2;
                this->columns[2] = refCol3;
                this->columns[3] = refCol4;
            }

            Matrix4x4(const Matrix3x3<ScalarType> &refMatrix);

            //Operators
            Matrix4x4 operator*(float32 right) const;
            vec4f32 operator*(const vec4f32 &refRight) const;
			vec4 operator*(const vec4 &rhs) const;
            Matrix4x4 operator*(const Matrix4x4 &refRight) const;

            //Inline operators
            inline float32 &operator()(uint8 row, uint8 col)
            {
                ASSERT(col < 4, "Column must be < 4");

                return this->columns[col][row];
            }

            inline const float32 &operator()(uint8 row, uint8 col) const
            {
                ASSERT(col < 4, "Column must be < 4");

                return this->columns[col][row];
            }

            inline vec4 &operator[](uint8 col)
            {
                ASSERT(col < 4, "Column must be < 4");

                return this->columns[col];
            }

            inline const vec4 &operator[](uint8 col) const
            {
                ASSERT(col < 4, "Column must be < 4");

                return this->columns[col];
            }

			inline Matrix4x4 &operator*=(const Matrix4x4 &refRight)
			{
				*this = *this * refRight;

				return *this;
			}

            //Methods
            Matrix4x4 Inverse() const;
            Matrix4x4 Transpose() const;

            //Functions
            static Matrix4x4 Identity();
            static Matrix4x4 LookAtRH(const vec3 &refCamPos, const vec3 &refCamTarget, const vec3 &refCamUpVector);
            static Matrix4x4 OrthographicRH(float32 left, float32 right, float32 bottom, float32 top, float32 zNear, float32 zFar);
            static Matrix4x4 PerspectiveRH(const Degree<ScalarType> &refFovY, float32 aspectRatio, float32 zNear, float32 zFar);
            static Matrix4x4 RotationX(const Radian<ScalarType> &refAngle);
            static Matrix4x4 RotationY(const Radian<ScalarType> &refAngle);
            static Matrix4x4 RotationYawPitchRoll(const Radian<ScalarType> &refYaw, const Radian<ScalarType> &refPitch, const Radian<ScalarType> &refRoll);
            static Matrix4x4 RotationZ(const Radian<ScalarType> &refAngle);
            static Matrix4x4 Scale(const vec3 &s);
            static Matrix4x4 Translation(const vec3 &t);

        private:
            //Members
            vec4 columns[4];
        };

		typedef Matrix4x4<float32> Matrix4s;
    }
}
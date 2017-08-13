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
#include "ACStdLib/Definitions.h"
#include "Radian.hpp"
#include "Vector4.hpp"

namespace ACStdLib
{
    //Forward declarations
    class vec4f32;

    namespace Math
    {
        //Forward declarations
        class Matrix3x3;
        class Vector3;
        class Vector4;

        class ACSTDLIB_API Matrix4x4
        {
        private:
            //Members
            Vector4 columns[4];

        public:
            //Constructors
            inline Matrix4x4()
            {
            }

            inline Matrix4x4(const Vector4 &refCol1, const Vector4 &refCol2, const Vector4 &refCol3, const Vector4 &refCol4)
            {
                this->columns[0] = refCol1;
                this->columns[1] = refCol2;
                this->columns[2] = refCol3;
                this->columns[3] = refCol4;
            }

            Matrix4x4(const Matrix3x3 &refMatrix);

            //Operators
            Matrix4x4 operator*(float32 right) const;
            vec4f32 operator*(const vec4f32 &refRight) const;
            Matrix4x4 operator*(const Matrix4x4 &refRight) const;
            Matrix4x4 &operator*=(const Matrix4x4 &refRight);

            //Inline operators
            inline float32 &operator()(uint8 row, uint8 col)
            {
                ASSERT(col < 4);

                return this->columns[col][row];
            }

            inline const float32 &operator()(uint8 row, uint8 col) const
            {
                ASSERT(col < 4);

                return this->columns[col][row];
            }

            inline Vector4 &operator[](uint8 col)
            {
                ASSERT(col < 4);

                return this->columns[col];
            }

            inline const Vector4 &operator[](uint8 col) const
            {
                ASSERT(col < 4);

                return this->columns[col];
            }

            //Methods
            Matrix4x4 Inverse() const;
            Matrix4x4 Transpose() const;

            //Functions
            static Matrix4x4 Identity();
            static Matrix4x4 LookAtRH(const Vector3 &refCamPos, const Vector3 &refCamTarget, const Vector3 &refCamUpVector);
            static Matrix4x4 OrthographicRH(float32 left, float32 right, float32 bottom, float32 top, float32 zNear, float32 zFar);
            static Matrix4x4 PerspectiveRH(const Degree &refFovY, float32 aspectRatio, float32 zNear, float32 zFar);
            static Matrix4x4 RotationX(const Radian &refAngle);
            static Matrix4x4 RotationY(const Radian &refAngle);
            static Matrix4x4 RotationYawPitchRoll(const Radian &refYaw, const Radian &refPitch, const Radian &refRoll);
            static Matrix4x4 RotationZ(const Radian &refAngle);
            static Matrix4x4 Scale(float32 scaleX, float32 scaleY, float32 scaleZ);
            static Matrix4x4 Translation(float32 dx, float32 dy, float32 dz);
            static Matrix4x4 Translation(const Vector3 &refVector);
        };

        //Binary operators
        ACSTDLIB_API Vector4 operator*(const Matrix4x4 &refLeft, const Vector4 &refRight);
    }
}
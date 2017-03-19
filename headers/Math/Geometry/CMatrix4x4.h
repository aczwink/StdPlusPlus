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
#include "../CRadian.h"
#include "CVector4.h"

namespace ACStdLib
{
    //Forward declarations
    class vec4f32;

    namespace Math
    {
        //Forward declarations
        class CMatrix3x3;
        class CVector3;
        class CVector4;

        class ACSTDLIB_API CMatrix4x4
        {
        private:
            //Members
            CVector4 columns[4];

        public:
            //Constructors
            inline CMatrix4x4()
            {
            }

            inline CMatrix4x4(const CVector4 &refCol1, const CVector4 &refCol2, const CVector4 &refCol3, const CVector4 &refCol4)
            {
                this->columns[0] = refCol1;
                this->columns[1] = refCol2;
                this->columns[2] = refCol3;
                this->columns[3] = refCol4;
            }

            CMatrix4x4(const CMatrix3x3 &refMatrix);

            //Operators
            CMatrix4x4 operator*(float32 right) const;
            vec4f32 operator*(const vec4f32 &refRight) const;
            CMatrix4x4 &operator*=(const CMatrix4x4 &refRight);

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

            inline CVector4 &operator[](uint8 col)
            {
                ASSERT(col < 4);

                return this->columns[col];
            }

            inline const CVector4 &operator[](uint8 col) const
            {
                ASSERT(col < 4);

                return this->columns[col];
            }

            //Methods
            CMatrix4x4 Inverse() const;
            CMatrix4x4 Transpose() const;

            //Functions
            static CMatrix4x4 Identity();
            static CMatrix4x4 LookAtRH(const CVector3 &refCamPos, const CVector3 &refCamTarget, const CVector3 &refCamUpVector);
            static CMatrix4x4 OrthographicRH(float32 left, float32 right, float32 bottom, float32 top, float32 zNear, float32 zFar);
            static CMatrix4x4 PerspectiveRH(const CDegree &refFovY, float32 aspectRatio, float32 zNear, float32 zFar);
            static CMatrix4x4 RotationX(const CRadian &refAngle);
            static CMatrix4x4 RotationY(const CRadian &refAngle);
            static CMatrix4x4 RotationYawPitchRoll(const CRadian &refYaw, const CRadian &refPitch, const CRadian &refRoll);
            static CMatrix4x4 RotationZ(const CRadian &refAngle);
            static CMatrix4x4 Scale(float32 scaleX, float32 scaleY, float32 scaleZ);
            static CMatrix4x4 Translation(float32 dx, float32 dy, float32 dz);
            static CMatrix4x4 Translation(const CVector3 &refVector);
        };

        //Binary operators
        ACSTDLIB_API CMatrix4x4 operator*(const CMatrix4x4 &refLeft, const CMatrix4x4 &refRight);
        ACSTDLIB_API CVector4 operator*(const CMatrix4x4 &refLeft, const CVector4 &refRight);
    }
}
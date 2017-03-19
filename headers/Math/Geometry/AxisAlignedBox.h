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
#include <ACStdLibCPUOpt.hpp>
#include "CMatrix4x4.h"
#include "CVector3.h"

namespace ACStdLib
{
    namespace Math
    {
        class ACSTDLIB_API AxisAlignedBox
        {
        public:
            //Members
            CVector3 min;
            CVector3 max;

            //Constructors
            inline AxisAlignedBox()
            {
            }

            inline AxisAlignedBox(const CVector3 &refMin, const CVector3 &refMax)
            {
                this->min = refMin;
                this->max = refMax;
            }

            //Methods
            bool Intersects(const vec4f32 &refOrigin, const vec4f32 &refDir) const;
            void Merge(const CVector3 &refV);
            float32 SquaredDistanceTo(const vec4f32 &refPoint) const;

            //Inline
            inline bool Contains(const vec4f32 &refPoint) const
            {
                return (this->min.x <= refPoint.x && refPoint.x <= this->max.x) &&
                       (this->min.y <= refPoint.y && refPoint.y <= this->max.y) &&
                       (this->min.z <= refPoint.z && refPoint.z <= this->max.z);
            }

            inline float32 DistanceTo(const vec4f32 &refPoint) const
            {
                return sqrtf(this->SquaredDistanceTo(refPoint));
            }

            inline CVector3 GetSize() const
            {
                return this->max - this->min;
            }

            inline float32 GetVolume() const
            {
                CVector3 v;

                v = this->GetSize();

                return v.x * v.y * v.z;
            }

            inline AxisAlignedBox Transform(const CMatrix4x4 &refTransform) const
            {
                return AxisAlignedBox(CVector3(refTransform * CVector4(this->min, 1)), CVector3(refTransform * CVector4(this->max, 1)));
            }
        };
    }
}
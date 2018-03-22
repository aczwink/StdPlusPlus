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
#include "../../Definitions.h"
#include <Std++CPUOpt.hpp>
#include "../Matrix4x4.hpp"
#include "../Vector3.hpp"

namespace StdPlusPlus
{
    namespace Math
    {
		template <typename ScalarType>
        class STDPLUSPLUS_API AxisAlignedBox
        {
            typedef Vector3<ScalarType> vec3;
			typedef Vector4<ScalarType> vec4;
			typedef Matrix4x4<ScalarType> mat4;
        public:
            //Members
			vec3 min;
			vec3 max;

            //Constructors
            inline AxisAlignedBox()
            {
            }

            inline AxisAlignedBox(const vec3 &refMin, const vec3 &refMax)
            {
                this->min = refMin;
                this->max = refMax;
            }

            //Methods
            bool Intersects(const vec4f32 &refOrigin, const vec4f32 &refDir) const;
            void Merge(const vec3 &refV);
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

            inline vec3 GetSize() const
            {
                return this->max - this->min;
            }

            inline float32 GetVolume() const
            {
				vec3 v;

                v = this->GetSize();

                return v.x * v.y * v.z;
            }

            inline AxisAlignedBox Transform(const mat4 &refTransform) const
            {
                return AxisAlignedBox<ScalarType>(vec3(refTransform * vec4(this->min, 1)), vec3(refTransform * vec4(this->max, 1)));
            }
        };
    }
}
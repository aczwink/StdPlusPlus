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
#include <Std++/Math/Vector4/Vector4.hpp>

namespace StdXX
{
    namespace Math
    {
        STDPLUSPLUS_API bool IntersectRayTriangle(const Vector4S &refOrigin, const Vector4S &refDir, const Vector4S &refV0, const Vector4S &refV1, const Vector4S &refV2, Vector4S &refIntersectionPointBaryCentric);
    }
}
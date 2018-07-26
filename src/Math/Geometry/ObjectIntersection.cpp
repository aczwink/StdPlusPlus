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
//Main header
#include <Std++/Math/Geometry/ObjectIntersection.hpp>
//Local
#include <Std++CPUOpt.hpp>
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;

//Definitions
#define EPSILON 0.000001f

//Namespace functions
bool Math::IntersectRayTriangle(const vec4f32 &refOrigin, const vec4f32 &refDir, const vec4f32 &refV0, const vec4f32 &refV1, const vec4f32 &refV2, vec4f32 &refIntersectionPointBaryCentric)
{
	float32 det, inv_det;
	vec4f32 e1, e2, p, distance, q;

	//M鰈ler-Trumbore

	e1 = refV1 - refV0;
	e2 = refV2 - refV0;

	p = Cross(refDir, e2);

	det = Dot(e1, p);

	if(Abs(det) < EPSILON)
		return false;
	inv_det = 1.0f / det;

	distance = refOrigin - refV0;

	//first barycentric coordinate
	refIntersectionPointBaryCentric.x = inv_det * Dot(distance, p);
	if(refIntersectionPointBaryCentric.x < 0 || refIntersectionPointBaryCentric.x > 1)
		return false;

	q = Cross(distance, e1);
	refIntersectionPointBaryCentric.y = inv_det * Dot(refDir, q);
	if(refIntersectionPointBaryCentric.y < 0 || (refIntersectionPointBaryCentric.x + refIntersectionPointBaryCentric.y) > 1)
		return false;

	refIntersectionPointBaryCentric.z = inv_det * Dot(e2, q);

	return refIntersectionPointBaryCentric.z >= 0;
}
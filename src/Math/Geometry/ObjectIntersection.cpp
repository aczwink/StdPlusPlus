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
#include <Std++/Math/Vector4/Vector4.inl>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;

//Definitions
#define EPSILON 0.000001f

//Namespace functions
bool Math::IntersectRayTriangle(const Vector4S &refOrigin, const Vector4S &refDir, const Vector4S &refV0, const Vector4S &refV1, const Vector4S &refV2, Vector4S &refIntersectionPointBaryCentric)
{
	float32 det, inv_det;
	Vector4S e1, e2, p, distance, q;

	//M鰈ler-Trumbore

	e1 = refV1 - refV0;
	e2 = refV2 - refV0;

	p = refDir.Cross(e2);

	det = e1.Dot(p);

	if(Abs(det) < EPSILON)
		return false;
	inv_det = 1.0f / det;

	distance = refOrigin - refV0;

	//first barycentric coordinate
	refIntersectionPointBaryCentric.x = inv_det * distance.Dot(p);
	if(refIntersectionPointBaryCentric.x < 0 || refIntersectionPointBaryCentric.x > 1)
		return false;

	q = distance.Cross(e1);
	refIntersectionPointBaryCentric.y = inv_det * refDir.Dot(q);
	if(refIntersectionPointBaryCentric.y < 0 || (refIntersectionPointBaryCentric.x + refIntersectionPointBaryCentric.y) > 1)
		return false;

	refIntersectionPointBaryCentric.z = inv_det * e2.Dot(q);

	return refIntersectionPointBaryCentric.z >= 0;
}
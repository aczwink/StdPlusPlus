/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
//Class header
#include <Std++/Math/Geometry/AxisAlignedBox.hpp>
//Local
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;

//Public methods
template <typename ScalarType>
bool AxisAlignedBox<ScalarType>::Intersects(const Vector4S &refOrigin, const Vector4S &refDir) const
{
	float32 tmin, tmax;
	Vector4S t0, t1;

	//implementation using the slab method

	t0 = (Vector4S(this->min, 0) - refOrigin) / refDir;
	t1 = (Vector4S(this->max, 0) - refOrigin) / refDir;

	//check x
	tmin = Min(t0.x, t1.x);
	tmax = Max(t0.x, t1.x);

	//check y
	tmin = Max(tmin, Min(t0.y, t1.y));
	tmax = Min(tmax, Max(t0.y, t1.y));

	//check z
	tmin = Max(tmin, Min(t0.z, t1.z));
	tmax = Min(tmax, Max(t0.z, t1.z));

	//t can't be negative as this would be behind the ray origin...

	return Max(tmin, ScalarType(0)) <= tmax;
}

template <typename ScalarType>
void AxisAlignedBox<ScalarType>::Merge(const Vector3<ScalarType> &refV)
{
	if(refV.x < this->min.x)
		this->min.x = refV.x;
	if(refV.y < this->min.y)
		this->min.y = refV.y;
	if(refV.z < this->min.z)
		this->min.z = refV.z;

	if(refV.x > this->max.x)
		this->max.x = refV.x;
	if(refV.y > this->max.y)
		this->max.y = refV.y;
	if(refV.z > this->max.z)
		this->max.z = refV.z;
}

template <typename ScalarType>
float32 AxisAlignedBox<ScalarType>::SquaredDistanceTo(const Vector4S &refPoint) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

namespace StdXX
{
	namespace Math
	{
		//Explicit instantiations
		template class STDPLUSPLUS_API AxisAlignedBox<float32>;
	}
}
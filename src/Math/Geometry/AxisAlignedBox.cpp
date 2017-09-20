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
//Class header
#include <ACStdLib/Math/Geometry/AxisAlignedBox.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;

//Public methods
bool AxisAlignedBox::Intersects(const vec4f32 &refOrigin, const vec4f32 &refDir) const
{
	float32 tmin, tmax;
	vec4f32 t0, t1;

	//implementation using the slab method

	t0 = (vec4f32(this->min, 0) - refOrigin) / refDir;
	t1 = (vec4f32(this->max, 0) - refOrigin) / refDir;

	//check x
	tmin = MIN(t0.x, t1.x);
	tmax = MAX(t0.x, t1.x);

	//check y
	tmin = MAX(tmin, MIN(t0.y, t1.y));
	tmax = MIN(tmax, MAX(t0.y, t1.y));

	//check z
	tmin = MAX(tmin, MIN(t0.z, t1.z));
	tmax = MIN(tmax, MAX(t0.z, t1.z));

	//t can't be negative as this would be behind the ray origin...

	return MAX(tmin, 0) <= tmax;
}
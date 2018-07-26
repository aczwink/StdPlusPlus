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
//Class header
#include <Std++/Rendering/VectorPath/VectorPath.hpp>
//Local
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Rendering;

//Local functions
static void TessellateBezier(const vec2f64 &refP0, const vec2f64 &refCP0, const vec2f64 &refCP1, const vec2f64 &refP1, uint8 level, FlatVectorPath &refCurrentPath, bool isCorner)
{
	vec2f64 p0cp0, cp0cp1, cp1p1, dir, normal, dirToCP0, dirToCP1, newCP0, newCP1, middle;
	float64 length[2];

	//use de casteljau to generate extra points

	if(level > 10) //not too many
		return;

	//calc middle points between (p0,cp0), (cp0, cp1), (cp1, p1)
	p0cp0 = (refP0 + refCP0) / 2;
	cp0cp1 = (refCP0 + refCP1) / 2;
	cp1p1 = (refCP1 + refP1) / 2;

	//TODO: I DONT FULLY UNDERSTAND THE FOLLOWING
	//WHAT IS IT WITH THE LENGTHS? and the 0.25 is the tessellation tolerance... what is that exactly?
	dir = refP1 - refP0;
	normal = GetPerpendicularDirection(dir);
	//directions from end point to control points
	dirToCP0 = refCP0 - refP1;
	dirToCP1 = refCP1 - refP1;

	length[0] = Math::Abs(Dot(dirToCP0, normal));
	length[1] = Math::Abs(Dot(dirToCP1, normal));

	if((length[0] + length[1]) * (length[0] + length[1]) < 0.25 * dir.LengthSquared())
	{
		refCurrentPath.AddPoint(refP1, isCorner);
		return;
	}

	//split the curve in the middle
	//calculate new control points
	newCP0 = (p0cp0 + cp0cp1) / 2;
	newCP1 = (cp0cp1 + cp1p1) / 2;
	middle = (newCP0 + newCP1) / 2;

	//tessellate left side of curve
	TessellateBezier(refP0, p0cp0, newCP0, middle, level+1, refCurrentPath, false);
	//tessellate right side of curve
	TessellateBezier(middle, newCP1, cp1p1, refP1, level+1, refCurrentPath, isCorner);
}

//Public methods
DynamicArray<FlatVectorPath *> VectorPath::Flatten()
{
	uint32 i, j;
	FlatVectorPath *pCurrentPath;
	DynamicArray<FlatVectorPath *> result;

	pCurrentPath = nullptr;

	j = 0;
	for(i = 0; i < this->commands.GetNumberOfElements(); i++)
	{
		switch(this->commands[i])
		{
			case ECommand::BezierTo:
			{
				TessellateBezier(this->points[j-1], this->points[j], this->points[j+1], this->points[j + 2], 0, *pCurrentPath, true);
				j += 3;
			}
				break;
			case ECommand::Close:
			{
				pCurrentPath->isClosed = true;
			}
				break;
			case ECommand::MoveTo:
			{
				//new sub path

				pCurrentPath = new FlatVectorPath;

				result.Push(pCurrentPath);

				pCurrentPath->AddCorner(this->points[j++]);
			}
				break;
			case ECommand::LineTo:
			{
				pCurrentPath->AddCorner(this->points[j++]);
			}
				break;
			default:
				NOT_IMPLEMENTED_ERROR;
		}
	}

	//
	for(FlatVectorPath *const& refpPath : result)
	{
		refpPath->ComputePointAttributes();
	}

	return result;
}
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
#include <Std++/Rendering/VectorPath/FlatVectorPath.hpp>
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX::Rendering;

//Public methods
void FlatVectorPath::ComputeJoins(float64 w, float64 miterLimit)
{
	uint32 index, prevIndex, i, nLeftTurns;
	float64 w_inverse, extrusionLengthSquared, limit;
	Math::Vector2D dl0, dl1;

	nLeftTurns = 0;
	w_inverse = 0;
	if(w > 0)
		w_inverse = 1.0 / w;

	prevIndex = this->points.GetNumberOfElements() - 1;
	index = 0;

	for(i = 0; i < this->points.GetNumberOfElements(); i++)
	{
		const FlatVectorPath::Point &refPrev = this->points[prevIndex];
		FlatVectorPath::Point &refCurrent = this->points[index];

		//get perpendicular directions
		dl0 = refPrev.dir.PerpendicularDirection();
		dl1 = refCurrent.dir.PerpendicularDirection();

		//calc extrusion
		refCurrent.extrusion = (dl0 + dl1) / 2;
		extrusionLengthSquared = refCurrent.extrusion.LengthSquared();
		/*
		if(extrusionLengthSquared > 0)
			refCurrent.extrusion *= float32(1.0 / extrusionLengthSquared);
		*/

		//keep track of left turns
		if(refCurrent.dir.Cross2D(refPrev.dir) < 0)
		{
			//this->points[i].isLeft = true;
			nLeftTurns++;
		}

		//should we use bevel or miter for inner join
		limit = Math::Min(refPrev.length, refCurrent.length) * w_inverse;
		if(limit < 1)
			limit = 1;
		if((extrusionLengthSquared * limit * limit) < 1.0)
			refCurrent.bevelInner = true;

		//check if corner needs to be beveled
		if(refCurrent.isCorner)
		{
			if((extrusionLengthSquared * miterLimit * miterLimit) < 1.0)
			{
				refCurrent.bevelOuter = true;
			}
		}

		/*
		if(this->points[i].bevelInner || this->points[i].bevelOuter)
			this->bevelCount++;
		*/

		prevIndex = index++;
	}

	this->isConvex = nLeftTurns == this->points.GetNumberOfElements();
}

void FlatVectorPath::ComputePointAttributes()
{
	uint32 i, index, nextIndex;

	//we need for every point to take the following point into account. This includes last point to first
	index = this->points.GetNumberOfElements() - 1;
	nextIndex = 0;

	//calc direction
	for(i = 0; i < this->points.GetNumberOfElements(); i++)
	{
		this->points[index].dir = this->points[nextIndex].pos - this->points[index].pos;

		this->points[index].length = this->points[index].dir.Length();
		this->points[index].dir = this->points[index].dir.Normalized();

		index = nextIndex++;
	}
}
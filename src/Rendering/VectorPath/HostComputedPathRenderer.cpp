/*
 * Copyright (c) 2017-2024 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Rendering/VectorPath/HostComputedPathRenderer.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::Rendering;

//Constructor
HostComputedPathRenderer::HostComputedPathRenderer()
{
	this->pCurrentPath = nullptr;

	//init current state
	this->state.transform = Matrix3S::Identity();
	this->state.lineCap = ELineCap::Butt;
	this->state.miterLimit = 10; //TODO: why this value?
	this->state.strokeColor = Color();
	this->state.strokeWidth = 1;
}

//Destructor
HostComputedPathRenderer::~HostComputedPathRenderer()
{
	this->ClearCache();
}

//Public methods
void HostComputedPathRenderer::Fill()
{
	//flatten paths
	if(this->pathCache.IsEmpty())
		this->pathCache = this->pCurrentPath->Flatten();
	this->pathAttributes.Resize(this->pathCache.GetNumberOfElements());

	//compute the bounding rectangle
	this->boundingRectMin.x = FLT_MAX;
	this->boundingRectMin.y = FLT_MAX;
	this->boundingRectMax.x = -FLT_MAX;
	this->boundingRectMax.y = -FLT_MAX;

	for(FlatVectorPath *const& refpPath : this->pathCache)
	{
		for(const FlatVectorPath::Point &refPoint : refpPath->GetPoints())
		{
			if(refPoint.pos.x < this->boundingRectMin.x)
				this->boundingRectMin.x = refPoint.pos.x;
			if(refPoint.pos.y < this->boundingRectMin.y)
				this->boundingRectMin.y = refPoint.pos.y;

			if(refPoint.pos.x > this->boundingRectMax.x)
				this->boundingRectMax.x = refPoint.pos.x;
			if(refPoint.pos.y > this->boundingRectMax.y)
				this->boundingRectMax.y = refPoint.pos.y;
		}
	}

	//compute all joins for paths
	for(FlatVectorPath *const& refpPath : this->pathCache)
	{
		refpPath->ComputeJoins(0, 2.4); //TODO: WHY THIS MITER LIMIT?
	}

	this->ExpandFill();
	this->RenderFill(this->pathCache, (DynamicArray<PathAttributes> &&)this->pathAttributes, this->boundingRectMin, this->boundingRectMax);
}

void HostComputedPathRenderer::Stroke()
{
	float64 w = this->state.strokeWidth / 2;

	//flatten paths
	if(this->pathCache.IsEmpty())
		this->pathCache = this->pCurrentPath->Flatten();
	this->pathAttributes.Resize(this->pathCache.GetNumberOfElements());

	//compute all joins for paths
	for(FlatVectorPath *const& refpPath : this->pathCache)
	{
		refpPath->ComputeJoins(w, this->state.miterLimit);
	}

	this->ExpandStroke(w);
	this->RenderStroke(this->pathCache, (DynamicArray<PathAttributes> &&)this->pathAttributes);
}

//Private methods
void HostComputedPathRenderer::BevelJoin(const FlatVectorPath::Point& point, const FlatVectorPath::Point& prev, float64 w)
{
	auto dl0 = prev.dir.PerpendicularDirection();
	auto dl1 = point.dir.PerpendicularDirection();

	if(point.isLeftTurn)
	{
		Vector2D l0, l1;
		this->ChooseBevel(point.bevelInner, point, prev, w, l0, l1);

		this->AddVertex(l0, {0, 1});
		this->AddVertex(point.pos - dl0 * w, {1, 1});

		if(point.bevelOuter)
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
		else
		{
			auto rx0 = point.pos - point.extrusion * w;

			this->AddVertex(point.pos, {0.5, 1});
			this->AddVertex(point.pos - dl0 * w, {1, 1});

			this->AddVertex(rx0, {1, 1});
			this->AddVertex(rx0, {1, 1});

			this->AddVertex(point.pos, {0.5, 1});
			this->AddVertex(point.pos - dl1 * w, {1, 1});
		}

		this->AddVertex(l1, {0, 1});
		this->AddVertex(point.pos - dl1 * w, {1, 1});
	}
	else
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
}

void HostComputedPathRenderer::ButtCapEnd(const FlatVectorPath::Point& point, float64 w)
{
	Math::Vector2D perpendicularDirection;

	//we calculate a direction perpendicular to the original one
	perpendicularDirection = Math::Vector2D(point.dir.y, -point.dir.x);

	this->AddVertex(point.pos.x + perpendicularDirection.x * w, point.pos.y + perpendicularDirection.y * w, 0, 1);
	this->AddVertex(point.pos.x - perpendicularDirection.x * w, point.pos.y - perpendicularDirection.y * w, 1, 1);
	this->AddVertex(point.pos.x + perpendicularDirection.x * w + point.dir.x, point.pos.y + perpendicularDirection.y * w + point.dir.y, 0, 0);
	this->AddVertex(point.pos.x - perpendicularDirection.x * w + point.dir.x, point.pos.y - perpendicularDirection.y * w + point.dir.y, 1, 0);
}

void HostComputedPathRenderer::ButtCapStart(const FlatVectorPath::Point &refPoint, float64 w)
{
	Math::Vector2D perpendicularDirection;

	/*
	Butt means that we have a square at the end point.
	Create four vertices around the start point
	*/

	//we calculate a direction perpendicular to the original one (always rightwards of arrow)
	perpendicularDirection = Math::Vector2D(refPoint.dir.y, -refPoint.dir.x);

	//top, left
	this->AddVertex(refPoint.pos.x + perpendicularDirection.x * w - refPoint.dir.x, refPoint.pos.y + perpendicularDirection.y * w - refPoint.dir.y, 0, 0);
	//top, right
	this->AddVertex(refPoint.pos.x - perpendicularDirection.x * w - refPoint.dir.x, refPoint.pos.y - perpendicularDirection.y * w - refPoint.dir.y, 1, 0);
	//bottom, left
	this->AddVertex(refPoint.pos.x + perpendicularDirection.x * w, refPoint.pos.y + perpendicularDirection.y * w, 0, 1);
	//bottom, right
	this->AddVertex(refPoint.pos.x - perpendicularDirection.x * w, refPoint.pos.y - perpendicularDirection.y * w, 1, 1);
}

void HostComputedPathRenderer::ChooseBevel(bool bevelInner, const FlatVectorPath::Point& point, const FlatVectorPath::Point& prev, float64 w, Vector2D& p0, Vector2D& p1)
{
	if(bevelInner)
	{
		p0 = point.pos + prev.dir.PerpendicularDirection() * w;
		p1 = point.pos + point.dir.PerpendicularDirection() * w;
	}
	else
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
}

void HostComputedPathRenderer::ClearCache()
{
	for(FlatVectorPath *const& refpFlatPath : this->pathCache)
		delete refpFlatPath;
	this->pathCache.Clear();

	if(this->pCurrentPath && this->freeCurrentPath)
		delete this->pCurrentPath;
	this->pCurrentPath = nullptr;
}

void HostComputedPathRenderer::ExpandFill()
{
	uint32 i, verticesOffset;

	for(i = 0; i < this->pathCache.GetNumberOfElements(); i++)
	{
		FlatVectorPath *const& refpPath = this->pathCache[i];

		verticesOffset = this->vertices.GetNumberOfElements();

		//add vertices
		for(const FlatVectorPath::Point &point : refpPath->GetPoints())
		{
			this->AddVertex(point.pos, {0.5, 1});
		}

		//fill out path attributes
		this->pathAttributes[i].verticesBeginIndex = verticesOffset;
		this->pathAttributes[i].nVertices = this->vertices.GetNumberOfElements() - verticesOffset;
	}
}

void HostComputedPathRenderer::ExpandStroke(float64 w)
{
	for(uint32 i = 0; i < this->pathCache.GetNumberOfElements(); i++)
	{
		FlatVectorPath *const& refpPath = this->pathCache[i];

		uint32 verticesOffset = this->vertices.GetNumberOfElements();

		//check if we need to add a cap at start. This we do if the path is not closed
		uint32 startIndex, endIndex, previousIndex;
		if(refpPath->IsClosed())
		{
			previousIndex = refpPath->GetPoints().GetNumberOfElements() - 1;
			startIndex = 0;
			endIndex = refpPath->GetPoints().GetNumberOfElements();
		}
		else
		{
			previousIndex = 0;
			startIndex = 1;
			endIndex = refpPath->GetPoints().GetNumberOfElements() - 1;

			switch(this->state.lineCap)
			{
				case ELineCap::Butt:
					this->ButtCapStart(refpPath->GetPoint(0), w);
					break;
				default:
					NOT_IMPLEMENTED_ERROR;
			}
		}

		//loop over points
		for(uint32 j = startIndex; j < endIndex; j++)
		{
			const FlatVectorPath::Point& point = refpPath->GetPoint(j);
			const FlatVectorPath::Point& prev = refpPath->GetPoint(previousIndex);

			if(point.bevelInner || point.bevelOuter)
				this->BevelJoin(point, prev, w);
			else
			{
				this->AddVertex(point.pos + point.extrusion * w, {0, 1});
				this->AddVertex(point.pos - point.extrusion * w, {0, 1});
			}

			previousIndex = j;
		}

		//check if we need to add a cap at end
		if(refpPath->IsClosed())
		{
			//loop to first point
			this->AddVertex(this->vertices[verticesOffset].position, {0, 1});
			this->AddVertex(this->vertices[verticesOffset + 1].position, {1, 1});
		}
		else
		{
			switch(this->state.lineCap)
			{
				case ELineCap::Butt:
					this->ButtCapEnd(refpPath->GetPoint(endIndex), w);
					break;
				default:
					NOT_IMPLEMENTED_ERROR;
			}
		}

		//fill out path attributes
		this->pathAttributes[i].verticesBeginIndex = verticesOffset;
		this->pathAttributes[i].nVertices = this->vertices.GetNumberOfElements() - verticesOffset;
	}
}
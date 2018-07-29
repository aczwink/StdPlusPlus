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
	this->state.transform = Matrix2s::Identity();
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
		for(const FlatVectorPath::SPoint &refPoint : refpPath->GetPoints())
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
	this->RenderFill(this->pathCache, (DynamicArray<SPathAttributes> &&)this->pathAttributes, this->boundingRectMin, this->boundingRectMax);
}

void HostComputedPathRenderer::Stroke()
{
	float64 w;

	w = this->state.strokeWidth / 2;

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
	this->RenderStroke(this->pathCache, (DynamicArray<SPathAttributes> &&)this->pathAttributes);
}

//Private methods
void HostComputedPathRenderer::ButtCapEnd(const FlatVectorPath::SPoint &refPoint, float64 w)
{
	Math::Vector2D perpendicularDirection;

	//we calculate a direction perpendicular to the original one
	perpendicularDirection = Math::Vector2D(refPoint.dir.y, -refPoint.dir.x);

	this->AddVertex(refPoint.pos.x + perpendicularDirection.x * w, refPoint.pos.y + perpendicularDirection.y * w);
	this->AddVertex(refPoint.pos.x - perpendicularDirection.x * w, refPoint.pos.y - perpendicularDirection.y * w);
	this->AddVertex(refPoint.pos.x + perpendicularDirection.x * w + refPoint.dir.x, refPoint.pos.y + perpendicularDirection.y * w + refPoint.dir.y);
	this->AddVertex(refPoint.pos.x - perpendicularDirection.x * w + refPoint.dir.x, refPoint.pos.y - perpendicularDirection.y * w + refPoint.dir.y);
}

void HostComputedPathRenderer::ButtCapStart(const FlatVectorPath::SPoint &refPoint, float64 w)
{
	Math::Vector2D perpendicularDirection;

	/*
	Butt means that we have a square at the end point.
	Create four vertices around the start point
	*/

	//we calculate a direction perpendicular to the original one (always rightwards of arrow)
	perpendicularDirection = Math::Vector2D(refPoint.dir.y, -refPoint.dir.x);

	//top, left
	this->AddVertex(refPoint.pos.x + perpendicularDirection.x * w - refPoint.dir.x, refPoint.pos.y + perpendicularDirection.y * w - refPoint.dir.y);
	//top, right
	this->AddVertex(refPoint.pos.x - perpendicularDirection.x * w - refPoint.dir.x, refPoint.pos.y - perpendicularDirection.y * w - refPoint.dir.y);
	//bottom, left
	this->AddVertex(refPoint.pos.x + perpendicularDirection.x * w, refPoint.pos.y + perpendicularDirection.y * w);
	//bottom, right
	this->AddVertex(refPoint.pos.x - perpendicularDirection.x * w, refPoint.pos.y - perpendicularDirection.y * w);
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
		for(const FlatVectorPath::SPoint &refPoint : refpPath->GetPoints())
		{
			this->AddVertex(refPoint.pos);
		}

		//fill out path attributes
		this->pathAttributes[i].verticesBeginIndex = verticesOffset;
		this->pathAttributes[i].nVertices = this->vertices.GetNumberOfElements() - verticesOffset;
	}
}

void HostComputedPathRenderer::ExpandStroke(float64 w)
{
	uint32 startIndex, endIndex, i, j, verticesOffset;

	for(i = 0; i < this->pathCache.GetNumberOfElements(); i++)
	{
		FlatVectorPath *const& refpPath = this->pathCache[i];

		verticesOffset = this->vertices.GetNumberOfElements();

		//check if we need to add a cap at start. This we do if the path is not closed
		if(refpPath->IsClosed())
		{
			startIndex = 0;
			endIndex = refpPath->GetPoints().GetNumberOfElements();
		}
		else
		{
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
		for(j = startIndex; j < endIndex; j++)
		{
			const FlatVectorPath::SPoint &refPoint = refpPath->GetPoint(j);

			if(refPoint.bevelInner || refPoint.bevelOuter)
			{
				NOT_IMPLEMENTED_ERROR;
			}
			else
			{
				this->AddVertex(refPoint.pos + refPoint.extrusion * w);
				this->AddVertex(refPoint.pos - refPoint.extrusion * w);
			}
		}

		//check if we need to add a cap at end
		if(refpPath->IsClosed())
		{
			//loop to first point
			this->AddVertex(this->vertices[verticesOffset].position);
			this->AddVertex(this->vertices[verticesOffset + 1].position);
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
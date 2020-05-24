/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Math/Matrix3x3.hpp>
#include "VectorPathRenderer.hpp"
#include "VectorPath.hpp"

namespace StdXX::Rendering
{
	/**
	 * This abstract class computes the vector paths on the host machine (e.g. CPU) and tesselates them,
	 * while leaving the rendering of them undefined.
	 * By deriving this class, only the rendering of the precomputed and tesselated areas has to be imnplemented.
	 */
	class STDPLUSPLUS_API HostComputedPathRenderer : public VectorPathRenderer
	{
		struct SRendererState
		{
			float64 miterLimit;
			ELineCap lineCap;
			Math::Matrix3S transform;

			Color fillColor;

			float64 strokeWidth;
			Color strokeColor;
		};

	protected:
		struct PathAttributes
		{
			uint32 verticesBeginIndex;
			uint32 nVertices;
		};

		struct Vertex
		{
			Math::Vector2S position;
		};

	public:
		//Constructor
		HostComputedPathRenderer();

		//Destructor
		~HostComputedPathRenderer();

		//Methods
		void Fill();
		void Stroke();

		//Inline
		inline void BeginPath()
		{
			this->ClearCache();
			this->pCurrentPath = new VectorPath;
			this->freeCurrentPath = true;
		}

		inline void BezierTo(const Math::Vector2D &refCP1, const Math::Vector2D &refCP2, const Math::Vector2D &refTo)
		{
			this->pCurrentPath->BezierTo(refCP1, refCP2, refTo);
		}

		inline void ClosePath()
		{
			this->pCurrentPath->Close();
		}

		inline void MoveTo(const Math::Vector2D &refV)
		{
			this->pCurrentPath->MoveTo(refV);
		}

		inline void LineTo(const Math::Vector2D &refV)
		{
			this->pCurrentPath->LineTo(refV);
		}

		inline void SetPath(VectorPath &refPath)
		{
			this->ClearCache();
			this->pCurrentPath = &refPath;
			this->freeCurrentPath = false;
		}

	protected:
		//Members
		SRendererState state;
		DynamicArray<Vertex> vertices;

		//Inline
		inline void AddVertex(float64 x, float64 y, float64 u, float64 v)
		{
			this->AddVertex(Math::Vector2D(x, y), Math::Vector2D(u, v));
		}

		inline void AddVertex(const Math::Vector2S &pos, const Math::Vector2S& uv)
		{
			this->vertices.Push({ pos });
		}

		inline void AddVertex(const Math::Vector2D& pos, const Math::Vector2D& uv)
		{
			this->AddVertex(Math::Vector2S(pos.x, pos.y), Math::Vector2S(uv.x, uv.y));
		}

	private:
		//Members
		VectorPath *pCurrentPath;
		bool freeCurrentPath;
		DynamicArray<FlatVectorPath *> pathCache;
		DynamicArray<PathAttributes> pathAttributes;
		Math::Vector2D boundingRectMin;
		Math::Vector2D boundingRectMax;

		//Abstract
		virtual void RenderFill(const DynamicArray<FlatVectorPath *> &refPaths, DynamicArray<PathAttributes> &&refPathAttributes, Math::Vector2D boundingRectMin, Math::Vector2D boundingRectMax) = 0;
		virtual void RenderStroke(const DynamicArray<FlatVectorPath *> &refPaths, DynamicArray<PathAttributes> &&refPathAttributes) = 0;

		//Methods
		void ButtCapEnd(const FlatVectorPath::Point &point, float64 w);
		void ButtCapStart(const FlatVectorPath::Point &refPoint, float64 w);
		void ClearCache();
		void ExpandFill();
		void ExpandStroke(float64 w);
	};
}
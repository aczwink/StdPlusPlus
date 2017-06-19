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
#pragma once
//Local
#include "ACStdLib/Math/Matrix2x2.hpp"
#include "A2DPathRenderer.h"
#include "VectorPath.hpp"

namespace ACStdLib
{
    namespace Rendering
    {
		/**
		 * This abstract class computes the vector paths on the host machine (e.g. CPU) and tesselates them,
		 * while leaving the rendering of them undefined.
		 * By deriving this class, only the rendering of the precomputed and tesselated areas has to be imnplemented.
		 */
        class ACSTDLIB_API HostComputedPathRenderer : public A2DPathRenderer
        {
            struct SRendererState
            {
                float64 miterLimit;
                ELineCap lineCap;
                Math::Matrix2x2 transform;

                Color fillColor;

                float64 strokeWidth;
                Color strokeColor;
            };

        protected:
            struct SPathAttributes
            {
                uint32 verticesBeginIndex;
                uint32 nVertices;
            };

            struct SVertex
            {
                Math::Vector2 position;
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

            inline void BezierTo(const vec2f64 &refCP1, const vec2f64 &refCP2, const vec2f64 &refTo)
            {
                this->pCurrentPath->BezierTo(refCP1, refCP2, refTo);
            }

            inline void ClosePath()
            {
                this->pCurrentPath->Close();
            }

            inline void MoveTo(const vec2f64 &refV)
            {
                this->pCurrentPath->MoveTo(refV);
            }

            inline void LineTo(const vec2f64 &refV)
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
			DynamicArray<SVertex> vertices;

			//Inline
			inline void AddVertex(float64 x, float64 y)
			{
				this->AddVertex(vec2f64(x, y));
			}

			inline void AddVertex(const vec2f64 &refPos)
			{
				this->vertices.Push({refPos});
			}

		private:
			//Members
			VectorPath *pCurrentPath;
			bool freeCurrentPath;
			DynamicArray<FlatVectorPath *> pathCache;
			DynamicArray<SPathAttributes> pathAttributes;
			Math::Vector2 boundingRectMin;
			Math::Vector2 boundingRectMax;

			//Abstract
			virtual void RenderFill(const DynamicArray<FlatVectorPath *> &refPaths, DynamicArray<SPathAttributes> &&refPathAttributes, vec2f64 boundingRectMin, vec2f64 boundingRectMax) = NULL;
			virtual void RenderStroke(const DynamicArray<FlatVectorPath *> &refPaths, DynamicArray<SPathAttributes> &&refPathAttributes) = NULL;

			//Methods
			void ButtCapEnd(const FlatVectorPath::SPoint &refPoint, float64 w);
			void ButtCapStart(const FlatVectorPath::SPoint &refPoint, float64 w);
			void ClearCache();
			void ExpandFill();
			void ExpandStroke(float64 w);
        };
    }
}
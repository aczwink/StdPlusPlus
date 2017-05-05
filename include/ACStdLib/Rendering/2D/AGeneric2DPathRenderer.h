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
#include "../../Math/Geometry/CMatrix2x2.h"
#include "A2DPathRenderer.h"
#include "CVectorPath.h"

namespace ACStdLib
{
    namespace Rendering
    {
        class ACSTDLIB_API AGeneric2DPathRenderer : public A2DPathRenderer
        {
            struct SRendererState
            {
                float64 miterLimit;
                ELineCap lineCap;
                Math::CMatrix2x2 transform;

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
                Math::CVector2 position;
            };

        private:
            //Members
            CVectorPath *pCurrentPath;
            bool freeCurrentPath;
            CArray<CFlatVectorPath *> pathCache;
            CArray<SPathAttributes> pathAttributes;
            vec2f64 boundingRectMin;
            vec2f64 boundingRectMax;

            //Abstract
            virtual void RenderFill(const CArray<CFlatVectorPath *> &refPaths, CArray<SPathAttributes> &&refPathAttributes, vec2f64 boundingRectMin, vec2f64 boundingRectMax) = NULL;
            virtual void RenderStroke(const CArray<CFlatVectorPath *> &refPaths, CArray<SPathAttributes> &&refPathAttributes) = NULL;

            //Methods
            void ButtCapEnd(const CFlatVectorPath::SPoint &refPoint, float64 w);
            void ButtCapStart(const CFlatVectorPath::SPoint &refPoint, float64 w);
            void ClearCache();
            void ExpandFill();
            void ExpandStroke(float64 w);

        protected:
            //Members
            SRendererState state;
            CArray<SVertex> vertices;

            //Inline
            inline void AddVertex(float64 x, float64 y)
            {
                this->AddVertex(vec2f64(x, y));
            }

            inline void AddVertex(const vec2f64 &refPos)
            {
                this->vertices.Push({refPos});
            }

        public:
            //Constructor
            AGeneric2DPathRenderer();

            //Destructor
            ~AGeneric2DPathRenderer();

            //Methods
            void Fill();
            void Stroke();

            //Inline
            inline void BeginPath()
            {
                this->ClearCache();
                this->pCurrentPath = new CVectorPath;
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

            inline void SetPath(CVectorPath &refPath)
            {
                this->ClearCache();
                this->pCurrentPath = &refPath;
                this->freeCurrentPath = false;
            }
        };
    }
}
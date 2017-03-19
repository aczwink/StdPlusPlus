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
#include "../CDeviceContext.h"
#include "../IInputState.h"
#include "../IShaderProgram.h"
#include "../IVertexBuffer.h"
#include "AGeneric2DPathRenderer.h"

namespace ACStdLib
{
    namespace Rendering
    {
        class ACSTDLIB_API C2DPathRenderer : public AGeneric2DPathRenderer
        {
            enum class ERenderMethod
            {
                Fill,
                FillConvex,
                Stroke
            };

            struct SRenderEntry
            {
                ERenderMethod renderMethod;
                CColor color;
                ITexture *pTexture;
                CArray<SPathAttributes> pathAttributes;
                uint32 boundingRectangleOffset;
            };

        private:
            //Members
            CLinkedList<SRenderEntry> renderCalls;
            IShaderProgram *pShaderProgram;
            IInputState *pInputState;
            IVertexBuffer *pVertexBuffer;
            ITexture *pTexture;
            Math::CVector2 viewSize;

            CDeviceContext &refDC;

            //Methods
            void InitRendering();
            void RenderFill(const CArray<CFlatVectorPath *> &refPaths, CArray<SPathAttributes> &&refPathAttributes, vec2f64 boundingRectMin, vec2f64 boundingRectMax);
            void RenderStroke(const CArray<CFlatVectorPath *> &refPaths, CArray<SPathAttributes> &&refPathAttributes);

        public:
            //Constructor
            C2DPathRenderer(CDeviceContext &refDC);

            //Destructor
            ~C2DPathRenderer();

            //Methods from parents
            using A2DPathRenderer::BezierTo;
            using A2DPathRenderer::MoveTo;
            using A2DPathRenderer::LineTo;

            //Methods
            void Sync();

            //Inline
            inline void Scale(float32 scaleX, float32 scaleY)
            {
                this->state.transform = Math::CMatrix2x2::Scale(scaleX, scaleY) * this->state.transform;
            }

            inline void SetFillColor(const CColor &refFillColor)
            {
                this->state.fillColor = refFillColor;
            }

            inline void SetFillTexture(ITexture *pTexture)
            {
                this->pTexture = pTexture;
            }

            inline void SetStrokeColor(const CColor &refStrokeColor)
            {
                this->state.strokeColor = refStrokeColor;
            }

            inline void SetStrokeWidth(float64 strokeWidth)
            {
                this->state.strokeWidth = strokeWidth;
            }

            inline void SetViewPort(uint16 width, uint16 height)
            {
                this->viewSize = Math::CVector2(width, height);
            }
        };
    }
}
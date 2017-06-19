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
#include "ACStdLib/Rendering/DeviceContext.hpp"
#include "ACStdLib/Rendering/InputState.hpp"
#include "ACStdLib/Rendering/ShaderProgram.hpp"
#include "ACStdLib/Rendering/VertexBuffer.hpp"
#include "HostComputedPathRenderer.hpp"

namespace ACStdLib
{
    namespace Rendering
    {
        class ACSTDLIB_API DevicePathRenderer : public HostComputedPathRenderer
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
                Color color;
                ITexture *pTexture;
                DynamicArray<SPathAttributes> pathAttributes;
                uint32 boundingRectangleOffset;
            };

        public:
            //Constructor
            DevicePathRenderer(DeviceContext &refDC);

            //Destructor
            ~DevicePathRenderer();

            //Methods from parents
            using A2DPathRenderer::BezierTo;
            using A2DPathRenderer::MoveTo;
            using A2DPathRenderer::LineTo;

            //Methods
            void Sync();

            //Inline
            inline void Scale(float32 scaleX, float32 scaleY)
            {
                this->state.transform = Math::Matrix2x2::Scale(scaleX, scaleY) * this->state.transform;
            }

            inline void SetFillColor(const Color &refFillColor)
            {
                this->state.fillColor = refFillColor;
            }

            inline void SetFillTexture(ITexture *pTexture)
            {
                this->pTexture = pTexture;
            }

            inline void SetStrokeColor(const Color &refStrokeColor)
            {
                this->state.strokeColor = refStrokeColor;
            }

            inline void SetStrokeWidth(float64 strokeWidth)
            {
                this->state.strokeWidth = strokeWidth;
            }

            inline void SetViewPort(const Size &size)
            {
                this->viewSize = Math::Vector2(size.width, size.height);
            }

        private:
            //Members
            LinkedList<SRenderEntry> renderCalls;
            ShaderProgram *pShaderProgram;
            InputState *pInputState;
            VertexBuffer *pVertexBuffer;
            ITexture *pTexture;
            Math::Vector2 viewSize;

            DeviceContext &refDC;

            //Methods
            void InitRendering();
            void RenderFill(const DynamicArray<FlatVectorPath *> &refPaths, DynamicArray<SPathAttributes> &&refPathAttributes, vec2f64 boundingRectMin, vec2f64 boundingRectMax);
            void RenderStroke(const DynamicArray<FlatVectorPath *> &refPaths, DynamicArray<SPathAttributes> &&refPathAttributes);
        };
    }
}
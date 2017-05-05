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
#include <ACStdLib/Definitions.h>
#include <ACStdLib/Color.h>
#include "IShader.h"

namespace ACStdLib
{
    namespace UI
    {
        //Forward declarations
        class RenderTargetWidget;
    }

    namespace Rendering
    {
        //Forward declarations
        class ICubeMap;
        class IFrameBuffer;
        class IIndexBuffer;
        class IInputState;
        class IShaderProgram;
        class ITexture;
        class ITexture2D;
        class IVertexBuffer;

        enum class ETestFunction
        {
            True,
            Less,
            LessOrEqual,
            NotEqual
        };

        enum class ETestEffect
        {
            DecrementWithWrapAround,
            IncrementWithWrapAround,
            KeepValue,
            SetToZero,
        };

        class ACSTDLIB_API DeviceContext
        {
        private:
            //Members
            void *systemHandle;
            void *deviceState;
            IInputState *pCurrentInputState;

            //Methods
            void BindOSContext() const;
            void CreateOSContext(const UI::RenderTargetWidget &refView, uint8 nSamples);
            void DestroyOSContext();
            void UnbindOSContext();

        public:
            //Constructor
            DeviceContext(const UI::RenderTargetWidget &refView);
            DeviceContext(const UI::RenderTargetWidget &refView, uint8 nSamples);

            //Destructor
            ~DeviceContext();

            //Methods
            void ClearColorBuffer(const Color &refColor);
            void ClearDepthBuffer();
            ICubeMap *CreateCubeMap();
            IFrameBuffer *CreateFrameBuffer();
            IIndexBuffer *CreateIndexBuffer();
            IInputState *CreateInputState();
            IShader *CreateShader(IShader::EShaderType type);
            IShaderProgram *CreateShaderProgram();
            ITexture2D *CreateTexture2D();
            IVertexBuffer *CreateVertexBuffer();
            void DrawTriangleFan(uint32 startVertexIndex, uint32 nVertices);
            void DrawTriangles(uint32 startVertexIndex, uint32 nTriangles);
            void DrawTrianglesIndexed();
            void DrawTriangleStrip(uint32 startVertexIndex, uint32 nVertices);
            void EnableBlending(bool enabled = true);
            void EnableColorBufferWriting(bool writeRed, bool writeGreen, bool writeBlue, bool writeAlpha);
            void EnableDepthBufferWriting(bool writeEnable);
            void EnableDepthTest(bool enabled = true);
            void EnableFaceCulling(bool enabled = true);
            void EnablePolygonFilling(bool enabled = true);
            void EnableStencilTest(bool enabled = true);
            uint32 GetNumberOfTextureUnits() const;
            void SetDepthTest(ETestFunction function);
            void SetFrameBuffer(IFrameBuffer *pFrameBuffer);
            void SetInputState(IInputState *pInputState);
            void SetProgram(IShaderProgram *pProgram);
            void SetStencilTest(ETestFunction function, uint32 referenceValue, uint32 mask);
            void SetStencilTestEffects(ETestEffect stencilTestFailedEffect, ETestEffect stencilTestPassedDepthTestFailedEffect, ETestEffect stencilAndDepthTestsPassedEffect);
            void SetStencilTestEffects(bool frontFaces, ETestEffect stencilTestFailedEffect, ETestEffect stencilTestPassedDepthTestFailedEffect, ETestEffect stencilAndDepthTestsPassedEffect);
            void SetStencilWriteMask(uint32 mask);
            void SetTexture(uint8 unit, ITexture *pTexture);
            void SetViewPort(uint16 width, uint16 height);
            void SwapBuffers();
        };
    }
}
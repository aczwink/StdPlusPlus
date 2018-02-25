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
#pragma once
//Local
#include "../Definitions.h"
#include "../Color.hpp"
#include "../Math/Geometry/Rect.hpp"
#include "Shader.hpp"

namespace StdPlusPlus
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
        class IndexBuffer;
        class InputState;
        class ShaderProgram;
        class ITexture;
        class Texture2D;
        class VertexBuffer;

        enum class TestFunction
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

        enum class AllocationPolicy
        {
            Static,
            Dynamic
        };

        class STDPLUSPLUS_API DeviceContext
        {
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
            IndexBuffer *CreateIndexBuffer(AllocationPolicy policy = AllocationPolicy::Static);
            InputState *CreateInputState();
            Shader *CreateShader(Shader::ShaderType type);
            ShaderProgram *CreateShaderProgram();
            Texture2D *CreateTexture2D();
            VertexBuffer *CreateVertexBuffer(AllocationPolicy policy = AllocationPolicy::Static);
            void DrawLines(uint32 startVertexIndex, uint32 nLines);
			void DrawPoints(uint32 startVertexIndex, uint32 count);
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
            void ReadDepthBuffer(const Rect &block, float32 *output);
            void SetDepthTest(TestFunction function);
            void SetFrameBuffer(IFrameBuffer *pFrameBuffer);
            void SetInputState(InputState *pInputState);
			void SetPointSize(uint32 size);
            void SetProgram(ShaderProgram *pProgram);
            void SetStencilTest(TestFunction function, uint32 referenceValue, uint32 mask);
            void SetStencilTestEffects(ETestEffect stencilTestFailedEffect, ETestEffect stencilTestPassedDepthTestFailedEffect, ETestEffect stencilAndDepthTestsPassedEffect);
            void SetStencilTestEffects(bool frontFaces, ETestEffect stencilTestFailedEffect, ETestEffect stencilTestPassedDepthTestFailedEffect, ETestEffect stencilAndDepthTestsPassedEffect);
            void SetStencilWriteMask(uint32 mask);
            void SetTexture(uint8 unit, ITexture *pTexture);
			void SetUploadAlignment(uint8 value);
            void SetViewPort(uint16 width, uint16 height);
            void SwapBuffers();

        private:
            //Members
            void *systemHandle;
            void *deviceState;
			uint32 screenFrameBufferId;
            InputState *currentInputState;

            //Methods
            void BindOSContext() const;
            void CreateOSContext(const UI::RenderTargetWidget &renderTargetWidget, uint8 nSamples);
            void DestroyOSContext();
            void UnbindOSContext();
        };
    }
}
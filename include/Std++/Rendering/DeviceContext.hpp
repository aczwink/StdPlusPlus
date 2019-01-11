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
#include <Std++/Containers/Array/FixedArray.hpp>
#include <Std++/Multimedia/Pixmap.hpp>
#include "../Definitions.h"
#include "../Color.hpp"
#include "../Math/Geometry/Rect.hpp"
#include "Shader.hpp"

namespace StdXX
{
    namespace UI
    {
        //Move declarations
        class RenderTargetWidget;
    }

    namespace Rendering
    {
        //Move declarations
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
			//Destructor
			virtual ~DeviceContext()
			{
			}

			//Abstract
			virtual void ClearColorBuffer(const Color &refColor) = 0;
			virtual void ClearDepthBuffer() = 0;
			virtual ICubeMap *CreateCubeMap() = 0;
			virtual IFrameBuffer *CreateFrameBuffer() = 0;
			virtual IndexBuffer *CreateIndexBuffer(AllocationPolicy policy = AllocationPolicy::Static) = 0;
			virtual InputState *CreateInputState() = 0;
			virtual Shader *CreateShader(Shader::ShaderType type) = 0;
			virtual ShaderProgram *CreateShaderProgram() = 0;
			virtual Texture2D *CreateTexture2D() = 0;
			virtual VertexBuffer *CreateVertexBuffer(AllocationPolicy policy = AllocationPolicy::Static) = 0;
			virtual void DrawLines(uint32 startVertexIndex, uint32 nLines) = 0;
			virtual void DrawPoints(uint32 startVertexIndex, uint32 count) = 0;
			virtual void DrawTriangleFan(uint32 startVertexIndex, uint32 nVertices) = 0;
			virtual void DrawTriangles(uint32 startVertexIndex, uint32 nTriangles) = 0;
			/**
			 * If nTriangles == Natural<uint32>::Max() (default), then the number of triangles is taken from the index buffer.
			 *
			 * @param startIndex
			 * @param nTriangles
			 */
			virtual void DrawTrianglesIndexed(uint32 startIndex = 0, uint32 nTriangles = Unsigned<uint32>::Max()) = 0;
			virtual void DrawTriangleStrip(uint32 startVertexIndex, uint32 nVertices) = 0;
			virtual void EnableBlending(bool enabled = true) = 0;
			virtual void EnableColorBufferWriting(bool writeRed, bool writeGreen, bool writeBlue, bool writeAlpha) = 0;
			virtual void EnableDepthBufferWriting(bool writeEnable) = 0;
			virtual void EnableDepthTest(bool enabled = true) = 0;
			virtual void EnableFaceCulling(bool enabled = true) = 0;
			virtual void EnablePolygonFilling(bool enabled = true) = 0;
			virtual void EnableStencilTest(bool enabled = true) = 0;
			virtual uint32 GetNumberOfTextureUnits() const = 0;
			virtual Multimedia::Pixmap *ReadColorBuffer(const Math::Rect<uint16> &rect) = 0;
			virtual void ReadDepthBuffer(const Math::Rect<uint16> &block, float32 *output) = 0;
			virtual void SetDepthTest(TestFunction function) = 0;
			virtual void SetDownloadAlignment(uint8 value) = 0;
            virtual void SetFrameBuffer(IFrameBuffer *pFrameBuffer) = 0;
            virtual void SetInputState(InputState *pInputState) = 0;
			virtual void SetPointSize(uint32 size) = 0;
            virtual void SetProgram(ShaderProgram *pProgram) = 0;
            virtual void SetStencilTest(TestFunction function, uint32 referenceValue, uint32 mask) = 0;
            virtual void SetStencilTestEffects(ETestEffect stencilTestFailedEffect, ETestEffect stencilTestPassedDepthTestFailedEffect, ETestEffect stencilAndDepthTestsPassedEffect) = 0;
            virtual void SetStencilTestEffects(bool frontFaces, ETestEffect stencilTestFailedEffect, ETestEffect stencilTestPassedDepthTestFailedEffect, ETestEffect stencilAndDepthTestsPassedEffect) = 0;
            virtual void SetStencilWriteMask(uint32 mask) = 0;
            virtual void SetTexture(uint8 unit, ITexture *pTexture) = 0;
			virtual void SetUploadAlignment(uint8 value) = 0;
            virtual void SetViewPort(uint16 width, uint16 height) = 0;
            virtual void SwapBuffers() = 0;
        };
    }
}
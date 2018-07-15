/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Rendering/DeviceContext.hpp>
#include "../GLFunctions.h"
#include "OpenGLInputState.hpp"

namespace _stdpp
{
	class OpenGLDeviceContext : public StdPlusPlus::Rendering::DeviceContext
	{
	public:
		//Constructor
		OpenGLDeviceContext();

		//Methods
		void ClearColorBuffer(const StdPlusPlus::Color &refColor) override;
		void ClearDepthBuffer() override;
		StdPlusPlus::Rendering::ICubeMap *CreateCubeMap() override;
		StdPlusPlus::Rendering::IFrameBuffer *CreateFrameBuffer() override;
		StdPlusPlus::Rendering::IndexBuffer *CreateIndexBuffer(StdPlusPlus::Rendering::AllocationPolicy policy) override;
		StdPlusPlus::Rendering::InputState *CreateInputState() override;
		StdPlusPlus::Rendering::Shader *CreateShader(StdPlusPlus::Rendering::Shader::ShaderType type) override;
		StdPlusPlus::Rendering::ShaderProgram *CreateShaderProgram() override;
		StdPlusPlus::Rendering::Texture2D *CreateTexture2D() override;
		StdPlusPlus::Rendering::VertexBuffer *CreateVertexBuffer(StdPlusPlus::Rendering::AllocationPolicy policy) override;
		void DrawLines(uint32 startVertexIndex, uint32 nLines) override;
		void DrawPoints(uint32 startVertexIndex, uint32 count) override;
		void DrawTriangleFan(uint32 startVertexIndex, uint32 nVertices) override;
		void DrawTriangles(uint32 startVertexIndex, uint32 nTriangles) override;
		void DrawTrianglesIndexed() override;
		void DrawTriangleStrip(uint32 startVertexIndex, uint32 nVertices) override;
		void EnableBlending(bool enabled) override;
		void EnableColorBufferWriting(bool writeRed, bool writeGreen, bool writeBlue, bool writeAlpha) override;
		void EnableDepthBufferWriting(bool writeEnable) override;
		void EnableDepthTest(bool enabled) override;
		void EnableFaceCulling(bool enabled) override;
		void EnablePolygonFilling(bool enabled) override;
		void EnableStencilTest(bool enabled) override;
		uint32 GetNumberOfTextureUnits() const override;
		FixedArray<byte> ReadColorBuffer(const StdPlusPlus::Rect &rect) override;
		void ReadDepthBuffer(const StdPlusPlus::Rect &block, float32 *output) override;
		void SetDepthTest(StdPlusPlus::Rendering::TestFunction function) override;
		void SetDownloadAlignment(uint8 value) override;
		void SetFrameBuffer(StdPlusPlus::Rendering::IFrameBuffer *pFrameBuffer) override;
		void SetInputState(StdPlusPlus::Rendering::InputState *pInputState) override;
		void SetPointSize(uint32 size) override;
		void SetProgram(StdPlusPlus::Rendering::ShaderProgram *pProgram) override;
		void SetStencilTest(StdPlusPlus::Rendering::TestFunction function, uint32 referenceValue, uint32 mask) override;
		void SetStencilTestEffects(StdPlusPlus::Rendering::ETestEffect stencilTestFailedEffect, StdPlusPlus::Rendering::ETestEffect stencilTestPassedDepthTestFailedEffect, StdPlusPlus::Rendering::ETestEffect stencilAndDepthTestsPassedEffect) override;
		void SetStencilTestEffects(bool frontFaces, StdPlusPlus::Rendering::ETestEffect stencilTestFailedEffect, StdPlusPlus::Rendering::ETestEffect stencilTestPassedDepthTestFailedEffect, StdPlusPlus::Rendering::ETestEffect stencilAndDepthTestsPassedEffect) override;
		void SetStencilWriteMask(uint32 mask) override;
		void SetTexture(uint8 unit, StdPlusPlus::Rendering::ITexture *pTexture) override;
		void SetUploadAlignment(uint8 value) override;
		void SetViewPort(uint16 width, uint16 height) override;

	protected:
		//Abstract
		virtual void Bind() const = 0;

		//Methods
		void Init();

	private:
		//Members
		OpenGLInputState *currentInputState;

		//Inline
		inline void ActivateLastTextureUnit()
		{
			glActiveTexture(GL_TEXTURE31); //see OpenGLDeviceContext::SetTexture
		}
	};
}
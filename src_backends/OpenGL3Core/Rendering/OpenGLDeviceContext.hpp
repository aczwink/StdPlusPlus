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
#include "../Definitions.hpp"
#include "../GL3Functions.hpp"

//Forward declarations
class OpenGLInputState;

namespace _stdxx_
{
	class OpenGLDeviceContext : public StdXX::Rendering::DeviceContext
	{
	public:
		//Members
		GLFunctions_3_0 glFuncs;

		//Constructor
		OpenGLDeviceContext();

		//Methods
		void ClearColorBuffer(const StdXX::Color &refColor) override;
		void ClearDepthBuffer() override;
		StdXX::Rendering::ICubeMap *CreateCubeMap() override;
		StdXX::Rendering::IFrameBuffer *CreateFrameBuffer() override;
		StdXX::Rendering::IndexBuffer *CreateIndexBuffer(StdXX::Rendering::AllocationPolicy policy) override;
		StdXX::Rendering::InputState *CreateInputState() override;
		StdXX::Rendering::Shader *CreateShader(StdXX::Rendering::Shader::ShaderType type) override;
		StdXX::Rendering::ShaderProgram *CreateShaderProgram() override;
		StdXX::Rendering::Texture2D *CreateTexture2D() override;
		StdXX::Rendering::VertexBuffer *CreateVertexBuffer(StdXX::Rendering::AllocationPolicy policy) override;
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
		StdXX::Multimedia::Pixmap *ReadColorBuffer(const StdXX::Math::Rect<uint16> &rect) override;
		void ReadDepthBuffer(const StdXX::Math::Rect<uint16> &block, float32 *output) override;
		void SetDepthTest(StdXX::Rendering::TestFunction function) override;
		void SetDownloadAlignment(uint8 value) override;
		void SetFrameBuffer(StdXX::Rendering::IFrameBuffer *pFrameBuffer) override;
		void SetInputState(StdXX::Rendering::InputState *pInputState) override;
		void SetPointSize(uint32 size) override;
		void SetProgram(StdXX::Rendering::ShaderProgram *pProgram) override;
		void SetStencilTest(StdXX::Rendering::TestFunction function, uint32 referenceValue, uint32 mask) override;
		void SetStencilTestEffects(StdXX::Rendering::ETestEffect stencilTestFailedEffect, StdXX::Rendering::ETestEffect stencilTestPassedDepthTestFailedEffect, StdXX::Rendering::ETestEffect stencilAndDepthTestsPassedEffect) override;
		void SetStencilTestEffects(bool frontFaces, StdXX::Rendering::ETestEffect stencilTestFailedEffect, StdXX::Rendering::ETestEffect stencilTestPassedDepthTestFailedEffect, StdXX::Rendering::ETestEffect stencilAndDepthTestsPassedEffect) override;
		void SetStencilWriteMask(uint32 mask) override;
		void SetTexture(uint8 unit, StdXX::Rendering::ITexture *pTexture) override;
		void SetUploadAlignment(uint8 value) override;
		void SetViewPort(uint16 width, uint16 height) override;

	protected:
		//Abstract
		virtual void Bind() const = 0;

		//Methods
		void Init(GL_EXT_LOADER extensionLoaderFunction);

	private:
		//Members
		OpenGLInputState *currentInputState;

		//Methods
		void LoadOpenGLExtensions_2_0(GLFunctions_2_0 &glFuncs, GL_EXT_LOADER extensionLoaderFunction);
		void LoadOpenGLExtensions_3_0(GLFunctions_3_0 &glFuncs, GL_EXT_LOADER extensionLoaderFunction);

		//Inline
		inline void ActivateLastTextureUnit()
		{
			this->glFuncs.glActiveTexture(GL_TEXTURE31); //see OpenGLDeviceContext::SetTexture
		}
	};
}
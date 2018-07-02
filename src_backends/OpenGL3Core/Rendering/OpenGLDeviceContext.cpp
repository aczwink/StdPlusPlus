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
//Class header
#include "OpenGLDeviceContext.hpp"
//Local
#include "OpenGLCubeMap.hpp"
#include "OpenGLFrameBuffer.hpp"
#include "OpenGLIndexBuffer.h"
#include "OpenGLShader.hpp"
#include "OpenGLShaderProgram.hpp"
#include "OpenGLTexture2D.hpp"
#include "OpenGLVertexBuffer.hpp"
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::Rendering;
using namespace _stdpp;

//Local functions
static inline uint32 MapTestEffect(ETestEffect effect)
{
	switch(effect)
	{
		case ETestEffect::DecrementWithWrapAround:
			return GL_DECR_WRAP;
		case ETestEffect::IncrementWithWrapAround:
			return GL_INCR_WRAP;
		case ETestEffect::KeepValue:
			return GL_KEEP;
		case ETestEffect::SetToZero:
			return GL_ZERO;
	}

	NOT_IMPLEMENTED_ERROR;
	return -1;
}

static inline uint32 MapTestFunction(TestFunction func)
{
	switch(func)
	{
		case TestFunction::Less:
			return GL_LESS;
		case TestFunction::LessOrEqual:
			return GL_LEQUAL;
		case TestFunction::NotEqual:
			return GL_NOTEQUAL;
		case TestFunction::True:
			return GL_ALWAYS;
	}

	NOT_IMPLEMENTED_ERROR;
	return -1;
}

//Constructor
OpenGLDeviceContext::OpenGLDeviceContext() : currentInputState(nullptr)
{
}

//Public methods
void OpenGLDeviceContext::ClearColorBuffer(const Color &refColor)
{
	this->Bind();
	glClearColor(refColor.r, refColor.g, refColor.b, refColor.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLDeviceContext::ClearDepthBuffer()
{
	this->Bind();
	glClear(GL_DEPTH_BUFFER_BIT);
}

ICubeMap *OpenGLDeviceContext::CreateCubeMap()
{
	this->Bind();
	this->ActivateLastTextureUnit();

	return new OpenGLCubeMap;
}

IFrameBuffer *OpenGLDeviceContext::CreateFrameBuffer()
{
	this->Bind();

	return new OpenGLFrameBuffer;
}

IndexBuffer *OpenGLDeviceContext::CreateIndexBuffer(AllocationPolicy policy)
{
	this->Bind();

	return new OpenGLIndexBuffer(policy);
}

InputState *OpenGLDeviceContext::CreateInputState()
{
	this->Bind();

	return new OpenGLInputState;
}

Shader *OpenGLDeviceContext::CreateShader(Shader::ShaderType type)
{
	this->Bind();

	return new OpenGLShader(type);
}

ShaderProgram *OpenGLDeviceContext::CreateShaderProgram()
{
	this->Bind();

	return new OpenGLShaderProgram;
}

Texture2D *OpenGLDeviceContext::CreateTexture2D()
{
	this->Bind();
	this->ActivateLastTextureUnit();

	return new OpenGLTexture2D;
}

VertexBuffer *OpenGLDeviceContext::CreateVertexBuffer(AllocationPolicy policy)
{
	this->Bind();
	return new OpenGLVertexBuffer(policy);
}

void OpenGLDeviceContext::DrawLines(uint32 startVertexIndex, uint32 nLines)
{
	this->Bind();
	this->currentInputState->Bind();

	glDrawArrays(GL_LINES, startVertexIndex, nLines * 2);
}

void OpenGLDeviceContext::DrawPoints(uint32 startVertexIndex, uint32 count)
{
	this->Bind();
	this->currentInputState->Bind();

	glDrawArrays(GL_POINTS, startVertexIndex, count);
}

void OpenGLDeviceContext::DrawTriangleFan(uint32 startVertexIndex, uint32 nVertices)
{
	this->Bind();
	this->currentInputState->Bind();

	glDrawArrays(GL_TRIANGLE_FAN, startVertexIndex, nVertices);
}

void OpenGLDeviceContext::DrawTriangles(uint32 startVertexIndex, uint32 nTriangles)
{
	this->Bind();
	this->currentInputState->Bind();

	glDrawArrays(GL_TRIANGLES, startVertexIndex, nTriangles * 3);
}

void OpenGLDeviceContext::DrawTrianglesIndexed()
{
	this->Bind();
	this->currentInputState->Bind();
	OpenGLIndexBuffer *pIndexBuffer = this->currentInputState->GetIndexBuffer();
	pIndexBuffer->Bind();

	if(pIndexBuffer->IndicesShort())
		glDrawElements(GL_TRIANGLES, pIndexBuffer->GetNumberOfIndices(), GL_UNSIGNED_SHORT, nullptr);
	else
		glDrawElements(GL_TRIANGLES, pIndexBuffer->GetNumberOfIndices(), GL_UNSIGNED_INT, nullptr);
}

void OpenGLDeviceContext::DrawTriangleStrip(uint32 startVertexIndex, uint32 nVertices)
{
	this->Bind();
	this->currentInputState->Bind();

	glDrawArrays(GL_TRIANGLE_STRIP, startVertexIndex, nVertices);
}

void OpenGLDeviceContext::EnableBlending(bool enabled)
{
	this->Bind();

	if(enabled)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //TODO: this should be configureable, but mostly this is used, so its fine for now
	}
	else
		glDisable(GL_BLEND);
}

void OpenGLDeviceContext::EnableColorBufferWriting(bool writeRed, bool writeGreen, bool writeBlue, bool writeAlpha)
{
	this->Bind();

	glColorMask(writeRed, writeGreen, writeBlue, writeBlue);
}

void OpenGLDeviceContext::EnableDepthBufferWriting(bool writeEnable)
{
	this->Bind();

	glDepthMask(writeEnable);
}

void OpenGLDeviceContext::EnableDepthTest(bool enabled)
{
	this->Bind();

	if(enabled)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void OpenGLDeviceContext::EnableFaceCulling(bool enabled)
{
	this->Bind();

	if(enabled)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void OpenGLDeviceContext::EnablePolygonFilling(bool enabled)
{
	this->Bind();

	if(enabled)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void OpenGLDeviceContext::EnableStencilTest(bool enabled)
{
	this->Bind();

	if(enabled)
		glEnable(GL_STENCIL_TEST);
	else
		glDisable(GL_STENCIL_TEST);
}

uint32 OpenGLDeviceContext::GetNumberOfTextureUnits() const
{
	this->Bind();

	GLint maxTextures;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextures);

	return static_cast<uint32>(maxTextures);
}

FixedArray<byte> OpenGLDeviceContext::ReadColorBuffer(const Rect &rect)
{
	this->Bind();

	this->SetDownloadAlignment(1);
	FixedArray<byte> colorData(3u * rect.width() * rect.height());
	glReadPixels(rect.x(), rect.y(), rect.width(), rect.height(), GL_RGB, GL_UNSIGNED_BYTE, &colorData[0]);

	return colorData;
}

void OpenGLDeviceContext::ReadDepthBuffer(const Rect &block, float32 *output)
{
	this->Bind();
	glReadPixels(block.x(), block.y(), block.width(), block.height(), GL_DEPTH_COMPONENT, GL_FLOAT, output);
}

void OpenGLDeviceContext::SetDepthTest(TestFunction function)
{
	this->Bind();

	glDepthFunc(MapTestFunction(function));
}

void OpenGLDeviceContext::SetDownloadAlignment(uint8 value)
{
	this->Bind();

	glPixelStorei(GL_PACK_ALIGNMENT, value);
}

void OpenGLDeviceContext::SetFrameBuffer(IFrameBuffer *pFrameBuffer)
{
	this->Bind();

	if(pFrameBuffer == nullptr)
	{
		NOT_IMPLEMENTED_ERROR; //TODO: new implementation
		//glBindFramebuffer(GL_FRAMEBUFFER, this->screenFrameBufferId);
		//OpenGLFrameBuffer::currentFBO = this->screenFrameBufferId;
	}
	else
		((OpenGLFrameBuffer *)pFrameBuffer)->Bind();
}

void OpenGLDeviceContext::SetInputState(InputState *pInputState)
{
	this->currentInputState = dynamic_cast<OpenGLInputState *>(pInputState);
}

void OpenGLDeviceContext::SetPointSize(uint32 size)
{
	this->Bind();

	glPointSize(size);
}

void OpenGLDeviceContext::SetProgram(ShaderProgram *pProgram)
{
	this->Bind();
	OpenGLShaderProgram *pShaderProgram = (OpenGLShaderProgram *)pProgram;
	pShaderProgram->Use();
}

void OpenGLDeviceContext::SetStencilTest(TestFunction function, uint32 referenceValue, uint32 mask)
{
	this->Bind();

	glStencilFunc(MapTestFunction(function), referenceValue, mask);
}

void OpenGLDeviceContext::SetStencilTestEffects(ETestEffect stencilTestFailedEffect,
												ETestEffect stencilTestPassedDepthTestFailedEffect,
												ETestEffect stencilAndDepthTestsPassedEffect)
{
	this->Bind();

	glStencilOp(MapTestEffect(stencilTestFailedEffect), MapTestEffect(stencilTestPassedDepthTestFailedEffect), MapTestEffect(stencilAndDepthTestsPassedEffect));
}

void OpenGLDeviceContext::SetStencilTestEffects(bool frontFaces, ETestEffect stencilTestFailedEffect,
												ETestEffect stencilTestPassedDepthTestFailedEffect,
												ETestEffect stencilAndDepthTestsPassedEffect)
{
	this->Bind();

	glStencilOpSeparate(frontFaces ? GL_FRONT : GL_BACK, MapTestEffect(stencilTestFailedEffect), MapTestEffect(stencilTestPassedDepthTestFailedEffect), MapTestEffect(stencilAndDepthTestsPassedEffect));
}

void OpenGLDeviceContext::SetStencilWriteMask(uint32 mask)
{
	this->Bind();

	glStencilMask(mask);
}

void OpenGLDeviceContext::SetTexture(uint8 unit, ITexture *pTexture)
{
	this->Bind();

	glActiveTexture(GL_TEXTURE0 + unit);

	switch(pTexture->GetType())
	{
		case ETextureType::CubeMap:
			glBindTexture(GL_TEXTURE_CUBE_MAP, ((OpenGLCubeMap *)pTexture)->GetId());
			break;
		case ETextureType::Texture_2D:
			glBindTexture(GL_TEXTURE_2D, ((OpenGLTexture2D *)pTexture)->GetId());
			break;
	}

	glActiveTexture(GL_TEXTURE31); //we use this as "dummy" texture, else textures get overwritten by others as soon as one "binds". Stupid open gl-.-
	//Only when DeviceContext::SetTexture is called glActiveTexture will call something different then GL_TEXTURE31 in order to set the texture
	//but then immediatly restores the dummy
}

void OpenGLDeviceContext::SetUploadAlignment(uint8 value)
{
	this->Bind();

	glPixelStorei(GL_UNPACK_ALIGNMENT, value);
}

void OpenGLDeviceContext::SetViewPort(uint16 width, uint16 height)
{
	this->Bind();

	glViewport(0, 0, width, height);
}

//Protected methods
void OpenGLDeviceContext::Init()
{
	this->Bind();
	this->ActivateLastTextureUnit();
}
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
#include "OpenGLIndexBuffer.hpp"
#include "OpenGLInputState.hpp"
#include "OpenGLShader.hpp"
#include "OpenGLShaderProgram.hpp"
#include "OpenGLTexture2D.hpp"
#include "OpenGLVertexBuffer.hpp"
//Namespaces
using namespace StdXX;
using namespace StdXX::Rendering;
using namespace _stdxx_;

//Global variables
uint32 g_currentTextureId = Natural<uint32>::Max();

//Class fields
thread_local OpenGLState OpenGLDeviceContext::state;

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
	this->glFuncs.glClearColor(refColor.r, refColor.g, refColor.b, refColor.a);
	this->glFuncs.glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLDeviceContext::ClearDepthBuffer()
{
	this->Bind();
	this->glFuncs.glClear(GL_DEPTH_BUFFER_BIT);
}

ICubeMap *OpenGLDeviceContext::CreateCubeMap()
{
	this->Bind();
	this->ActivateLastTextureUnit();

	return new OpenGLCubeMap(*this);
}

IFrameBuffer *OpenGLDeviceContext::CreateFrameBuffer()
{
	this->Bind();

	return new OpenGLFrameBuffer(*this);
}

IndexBuffer *OpenGLDeviceContext::CreateIndexBuffer(AllocationPolicy policy)
{
	this->Bind();

	return new OpenGLIndexBuffer(policy, *this);
}

InputState *OpenGLDeviceContext::CreateInputState()
{
	this->Bind();

	return new OpenGLInputState(*this);
}

Shader *OpenGLDeviceContext::CreateShader(Shader::ShaderType type)
{
	this->Bind();

	return new OpenGLShader(type, *this);
}

ShaderProgram *OpenGLDeviceContext::CreateShaderProgram()
{
	this->Bind();

	return new OpenGLShaderProgram(*this);
}

Texture2D *OpenGLDeviceContext::CreateTexture2D()
{
	this->Bind();
	this->ActivateLastTextureUnit();

	return new OpenGLTexture2D(*this);
}

VertexBuffer *OpenGLDeviceContext::CreateVertexBuffer(AllocationPolicy policy)
{
	this->Bind();
	return new OpenGLVertexBuffer(policy, *this);
}

void OpenGLDeviceContext::DrawLines(uint32 startVertexIndex, uint32 nLines)
{
	this->Bind();
	this->currentInputState->Bind();

	this->glFuncs.glDrawArrays(GL_LINES, startVertexIndex, nLines * 2);
}

void OpenGLDeviceContext::DrawPoints(uint32 startVertexIndex, uint32 count)
{
	this->Bind();
	this->currentInputState->Bind();

	this->glFuncs.glDrawArrays(GL_POINTS, startVertexIndex, count);
}

void OpenGLDeviceContext::DrawTriangleFan(uint32 startVertexIndex, uint32 nVertices)
{
	this->Bind();
	this->currentInputState->Bind();

	this->glFuncs.glDrawArrays(GL_TRIANGLE_FAN, startVertexIndex, nVertices);
}

void OpenGLDeviceContext::DrawTriangles(uint32 startVertexIndex, uint32 nTriangles)
{
	this->Bind();
	this->currentInputState->Bind();

	this->glFuncs.glDrawArrays(GL_TRIANGLES, startVertexIndex, nTriangles * 3);
}

void OpenGLDeviceContext::DrawTrianglesIndexed(uint32 startIndex, uint32 nTriangles)
{
	this->Bind();
	this->currentInputState->Bind();
	const OpenGLIndexBuffer *openGLIndexBuffer = this->currentInputState->GetIndexBuffer();
	openGLIndexBuffer->Bind();

	uint32 nIndices;
	if(nTriangles == Natural<uint32>::Max())
		nIndices = openGLIndexBuffer->GetNumberOfIndices() - startIndex;
	else
		nIndices = nTriangles * 3;

	GLenum indexType;
	uint32 offset;
	if(openGLIndexBuffer->IndicesShort())
	{
		indexType = GL_UNSIGNED_SHORT;
		offset = startIndex * 2;
	}
	else
	{
		indexType = GL_UNSIGNED_INT;
		offset = startIndex * 4;
	}

	this->glFuncs.glDrawElements(GL_TRIANGLES, nIndices, indexType, reinterpret_cast<const void *>(offset));
}

void OpenGLDeviceContext::DrawTriangleStrip(uint32 startVertexIndex, uint32 nVertices)
{
	this->Bind();
	this->currentInputState->Bind();

	this->glFuncs.glDrawArrays(GL_TRIANGLE_STRIP, startVertexIndex, nVertices);
}

void OpenGLDeviceContext::EnableBlending(bool enabled)
{
	this->Bind();

	if(enabled)
	{
		this->glFuncs.glEnable(GL_BLEND);
		this->glFuncs.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //TODO: this should be configureable, but mostly this is used, so its fine for now
	}
	else
		this->glFuncs.glDisable(GL_BLEND);
}

void OpenGLDeviceContext::EnableColorBufferWriting(bool writeRed, bool writeGreen, bool writeBlue, bool writeAlpha)
{
	this->Bind();

	this->glFuncs.glColorMask(writeRed, writeGreen, writeBlue, writeBlue);
}

void OpenGLDeviceContext::EnableDepthBufferWriting(bool writeEnable)
{
	this->Bind();

	this->glFuncs.glDepthMask(writeEnable);
}

void OpenGLDeviceContext::EnableDepthTest(bool enabled)
{
	this->Bind();

	if(enabled)
		this->glFuncs.glEnable(GL_DEPTH_TEST);
	else
		this->glFuncs.glDisable(GL_DEPTH_TEST);
}

void OpenGLDeviceContext::EnableFaceCulling(bool enabled)
{
	this->Bind();

	if(enabled)
		this->glFuncs.glEnable(GL_CULL_FACE);
	else
		this->glFuncs.glDisable(GL_CULL_FACE);
}

void OpenGLDeviceContext::EnablePolygonFilling(bool enabled)
{
	this->Bind();

	if(enabled)
		this->glFuncs.glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		this->glFuncs.glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void OpenGLDeviceContext::EnableStencilTest(bool enabled)
{
	this->Bind();

	if(enabled)
		this->glFuncs.glEnable(GL_STENCIL_TEST);
	else
		this->glFuncs.glDisable(GL_STENCIL_TEST);
}

uint32 OpenGLDeviceContext::GetNumberOfTextureUnits() const
{
	this->Bind();

	GLint maxTextures;
	this->glFuncs.glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextures);

	return static_cast<uint32>(maxTextures);
}

Multimedia::Pixmap *OpenGLDeviceContext::ReadColorBuffer(const StdXX::Math::Rect<uint16> &rect)
{
	this->Bind();

	Multimedia::Pixmap *pixmap = new Multimedia::Pixmap(rect.size, Multimedia::NamedPixelFormat::RGB_24);

	this->SetDownloadAlignment(1);
	this->glFuncs.glReadPixels(rect.x(), rect.y(), rect.width(), rect.height(), GL_RGB, GL_UNSIGNED_BYTE, pixmap->GetPlane(0));

	return pixmap;
}

void OpenGLDeviceContext::ReadDepthBuffer(const StdXX::Math::Rect<uint16> &block, float32 *output)
{
	this->Bind();
	this->glFuncs.glReadPixels(block.x(), block.y(), block.width(), block.height(), GL_DEPTH_COMPONENT, GL_FLOAT, output);
}

void OpenGLDeviceContext::SetDepthTest(TestFunction function)
{
	this->Bind();

	this->glFuncs.glDepthFunc(MapTestFunction(function));
}

void OpenGLDeviceContext::SetDownloadAlignment(uint8 value)
{
	this->Bind();

	this->glFuncs.glPixelStorei(GL_PACK_ALIGNMENT, value);
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

	this->glFuncs.glPointSize(size);
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

	this->glFuncs.glStencilFunc(MapTestFunction(function), referenceValue, mask);
}

void OpenGLDeviceContext::SetStencilTestEffects(ETestEffect stencilTestFailedEffect,
												ETestEffect stencilTestPassedDepthTestFailedEffect,
												ETestEffect stencilAndDepthTestsPassedEffect)
{
	this->Bind();

	this->glFuncs.glStencilOp(MapTestEffect(stencilTestFailedEffect), MapTestEffect(stencilTestPassedDepthTestFailedEffect), MapTestEffect(stencilAndDepthTestsPassedEffect));
}

void OpenGLDeviceContext::SetStencilTestEffects(bool frontFaces, ETestEffect stencilTestFailedEffect,
												ETestEffect stencilTestPassedDepthTestFailedEffect,
												ETestEffect stencilAndDepthTestsPassedEffect)
{
	this->Bind();

	this->glFuncs.glStencilOpSeparate(frontFaces ? GL_FRONT : GL_BACK, MapTestEffect(stencilTestFailedEffect), MapTestEffect(stencilTestPassedDepthTestFailedEffect), MapTestEffect(stencilAndDepthTestsPassedEffect));
}

void OpenGLDeviceContext::SetStencilWriteMask(uint32 mask)
{
	this->Bind();

	this->glFuncs.glStencilMask(mask);
}

void OpenGLDeviceContext::SetTexture(uint8 unit, ITexture *pTexture)
{
	this->Bind();

	this->glFuncs.glActiveTexture(GL_TEXTURE0 + unit);

	switch(pTexture->GetType())
	{
		case ETextureType::CubeMap:
			this->glFuncs.glBindTexture(GL_TEXTURE_CUBE_MAP, ((OpenGLCubeMap *)pTexture)->GetId());
			break;
		case ETextureType::Texture_2D:
			this->glFuncs.glBindTexture(GL_TEXTURE_2D, ((OpenGLTexture2D *)pTexture)->GetId());
			break;
	}

	this->glFuncs.glActiveTexture(GL_TEXTURE31); //we use this as "dummy" texture, else textures get overwritten by others as soon as one "binds". Stupid open gl-.-
	//Only when DeviceContext::SetTexture is called glActiveTexture will call something different then GL_TEXTURE31 in order to set the texture
	//but then immediatly restores the dummy
}

void OpenGLDeviceContext::SetUploadAlignment(uint8 value)
{
	this->Bind();

	this->glFuncs.glPixelStorei(GL_UNPACK_ALIGNMENT, value);
}

void OpenGLDeviceContext::SetViewPort(uint16 width, uint16 height)
{
	this->Bind();

	this->glFuncs.glViewport(0, 0, width, height);
}

//Protected methods
void OpenGLDeviceContext::Init(GL_EXT_LOADER extensionLoaderFunction)
{
	this->Bind();
	this->LoadOpenGLExtensions_3_0(this->glFuncs, extensionLoaderFunction);
	this->ActivateLastTextureUnit();
}

//Private methods
void OpenGLDeviceContext::LoadOpenGLExtensions_2_0(GLFunctions_2_0 &glFuncs, GL_EXT_LOADER extensionLoaderFunction)
{
	glFuncs.glActiveTexture = (PFNGLACTIVETEXTUREPROC)extensionLoaderFunction(u8"glActiveTexture");
	glFuncs.glAttachShader = (PFNGLATTACHSHADERPROC)extensionLoaderFunction(u8"glAttachShader");

	glFuncs.glBindBuffer = (PFNGLBINDBUFFERPROC)extensionLoaderFunction(u8"glBindBuffer");
	glFuncs.glBindTexture = (PFNGLBINDTEXTUREPROC)extensionLoaderFunction(u8"glBindTexture");
	glFuncs.glBlendFunc = (PFNGLBLENDFUNCPROC)extensionLoaderFunction(u8"glBlendFunc");
	glFuncs.glBufferData = (PFNGLBUFFERDATAPROC)extensionLoaderFunction(u8"glBufferData");
	glFuncs.glBufferSubData = (PFNGLBUFFERSUBDATAPROC)extensionLoaderFunction(u8"glBufferSubData");

	glFuncs.glClear = (PFNGLCLEARPROC)extensionLoaderFunction(u8"glClear");
	glFuncs.glClearColor = (PFNGLCLEARCOLORPROC)extensionLoaderFunction(u8"glClearColor");
	glFuncs.glColorMask = (PFNGLCOLORMASKPROC)extensionLoaderFunction(u8"glColorMask");
	glFuncs.glCompileShader = (PFNGLCOMPILESHADERPROC)extensionLoaderFunction(u8"glCompileShader");
	glFuncs.glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)extensionLoaderFunction(u8"glCompressedTexImage2D");
	glFuncs.glCreateProgram = (PFNGLCREATEPROGRAMPROC)extensionLoaderFunction(u8"glCreateProgram");
	glFuncs.glCreateShader = (PFNGLCREATESHADERPROC)extensionLoaderFunction(u8"glCreateShader");

	glFuncs.glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)extensionLoaderFunction(u8"glDeleteBuffers");
	glFuncs.glDeleteProgram = (PFNGLDELETEPROGRAMPROC)extensionLoaderFunction(u8"glDeleteProgram");
	glFuncs.glDeleteShader = (PFNGLDELETESHADERPROC)extensionLoaderFunction(u8"glDeleteShader");
	glFuncs.glDeleteTextures = (PFNGLDELETETEXTURESPROC)extensionLoaderFunction(u8"glDeleteTextures");
	glFuncs.glDepthFunc = (PFNGLDEPTHFUNCPROC)extensionLoaderFunction(u8"glDepthFunc");
	glFuncs.glDepthMask = (PFNGLDEPTHMASKPROC)extensionLoaderFunction(u8"glDepthMask");
	glFuncs.glDetachShader = (PFNGLDETACHSHADERPROC)extensionLoaderFunction(u8"glDetachShader");
	glFuncs.glDisable = (PFNGLDISABLEPROC)extensionLoaderFunction(u8"glDisable");
	glFuncs.glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)extensionLoaderFunction(u8"glDisableVertexAttribArray");
	glFuncs.glDrawArrays = (PFNGLDRAWARRAYSPROC)extensionLoaderFunction(u8"glDrawArrays");
	glFuncs.glDrawBuffer = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)extensionLoaderFunction(u8"glDrawBuffer");
	glFuncs.glDrawElements = (PFNGLDRAWELEMENTSPROC)extensionLoaderFunction(u8"glDrawElements");

	glFuncs.glEnable = (PFNGLENABLEVERTEXATTRIBARRAYPROC)extensionLoaderFunction(u8"glEnable");
	glFuncs.glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)extensionLoaderFunction(u8"glEnableVertexAttribArray");

	glFuncs.glGenBuffers = (PFNGLGENBUFFERSPROC)extensionLoaderFunction(u8"glGenBuffers");
	glFuncs.glGenTextures = (PFNGLGENVERTEXARRAYSPROC)extensionLoaderFunction(u8"glGenTextures");
	glFuncs.glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)extensionLoaderFunction(u8"glGetAttribLocation");
	glFuncs.glGetError = (PFNGLGETERRORPROC)extensionLoaderFunction(u8"glGetError");
	glFuncs.glGetIntegerv = (PFNGLGETINTEGERVPROC)extensionLoaderFunction(u8"glGetIntegerv");
	glFuncs.glGetProgramiv = (PFNGLGETSHADERIVPROC)extensionLoaderFunction(u8"glGetProgramiv");
	glFuncs.glGetProgramInfoLog = (PFNGLGETSHADERINFOLOGPROC)extensionLoaderFunction(u8"glGetProgramInfoLog");
	glFuncs.glGetShaderiv = (PFNGLGETSHADERIVPROC)extensionLoaderFunction(u8"glGetShaderiv");
	glFuncs.glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)extensionLoaderFunction(u8"glGetShaderInfoLog");
	glFuncs.glGetTexParameteriv = (PFNGLGETTEXPARAMETERIVPROC)extensionLoaderFunction(u8"glGetTexParameteriv");
	glFuncs.glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)extensionLoaderFunction(u8"glGetUniformLocation");

	glFuncs.glLinkProgram = (PFNGLLINKPROGRAMPROC)extensionLoaderFunction(u8"glLinkProgram");

	glFuncs.glPixelStorei = (PFNGLPIXELSTOREIPROC)extensionLoaderFunction(u8"glPixelStorei");
	glFuncs.glPolygonMode = (PFNGLPOLYGONMODEPROC)extensionLoaderFunction(u8"glPolygonMode");
	glFuncs.glPointSize = (PFNGLPOINTSIZEPROC)extensionLoaderFunction(u8"glPointSize");

	glFuncs.glReadBuffer = (PFNGLREADBUFFERPROC)extensionLoaderFunction(u8"glReadBuffer");
	glFuncs.glReadPixels = (PFNGLREADPIXELSPROC)extensionLoaderFunction(u8"glReadPixels");

	glFuncs.glShaderSource = (PFNGLSHADERSOURCEPROC)extensionLoaderFunction(u8"glShaderSource");
	glFuncs.glStencilFunc = (PFNGLSTENCILFUNCPROC)extensionLoaderFunction(u8"glStencilFunc");
	glFuncs.glStencilMask = (PFNGLSTENCILMASKPROC)extensionLoaderFunction(u8"glStencilMask");
	glFuncs.glStencilOp = (PFNGLSTENCILOPPROC)extensionLoaderFunction(u8"glStencilOp");
	glFuncs.glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)extensionLoaderFunction(u8"glStencilOpSeparate");

	glFuncs.glTexImage2D = (PFNGLTEXIMAGE2DPROC)extensionLoaderFunction(u8"glTexImage2D");
	glFuncs.glTexParameteri = (PFNGLTEXPARAMETERIPROC)extensionLoaderFunction(u8"glTexParameteri");
	glFuncs.glTexSubImage2D = (PFNGLTEXIMAGE2DPROC)extensionLoaderFunction(u8"glTexSubImage2D");

	glFuncs.glUseProgram = (PFNGLUSEPROGRAMPROC)extensionLoaderFunction(u8"glUseProgram");
	glFuncs.glUniform1f = (PFNGLUNIFORM1FPROC)extensionLoaderFunction(u8"glUniform1f");
	glFuncs.glUniform1i = (PFNGLUNIFORM1IPROC)extensionLoaderFunction(u8"glUniform1i");
	glFuncs.glUniform2fv = (PFNGLUNIFORM2FVPROC)extensionLoaderFunction(u8"glUniform2fv");
	glFuncs.glUniform3fv = (PFNGLUNIFORM3FVPROC)extensionLoaderFunction(u8"glUniform3fv");
	glFuncs.glUniform4fv = (PFNGLUNIFORM4FVPROC)extensionLoaderFunction(u8"glUniform4fv");
	glFuncs.glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)extensionLoaderFunction(u8"glUniformMatrix2fv");
	glFuncs.glUniformMatrix3fv = (PFNGLUNIFORMMATRIX4FVPROC)extensionLoaderFunction(u8"glUniformMatrix3fv");
	glFuncs.glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)extensionLoaderFunction(u8"glUniformMatrix4fv");

	glFuncs.glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)extensionLoaderFunction(u8"glVertexAttribPointer");
	glFuncs.glViewport = (PFNGLVIEWPORTPROC)extensionLoaderFunction(u8"glViewport");
}

void OpenGLDeviceContext::LoadOpenGLExtensions_3_0(GLFunctions_3_0 &glFuncs, GL_EXT_LOADER extensionLoaderFunction)
{
	this->LoadOpenGLExtensions_2_0(glFuncs, extensionLoaderFunction);

	glFuncs.glBindFramebuffer = (PFNGLBINDBUFFERPROC)extensionLoaderFunction(u8"glBindFramebuffer");
	glFuncs.glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)extensionLoaderFunction(u8"glBindRenderbuffer");
	glFuncs.glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)extensionLoaderFunction(u8"glBindVertexArray");

	glFuncs.glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)extensionLoaderFunction(u8"glCheckFramebufferStatus");

	glFuncs.glDeleteFramebuffers = (PFNGLDELETEBUFFERSPROC)extensionLoaderFunction(u8"glDeleteFramebuffers");
	glFuncs.glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)extensionLoaderFunction(u8"glDeleteRenderbuffers");
	glFuncs.glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)extensionLoaderFunction(u8"glDeleteVertexArrays");

	glFuncs.glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)extensionLoaderFunction(u8"glFramebufferRenderbuffer");
	glFuncs.glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)extensionLoaderFunction(u8"glFramebufferTexture2D");

	glFuncs.glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)extensionLoaderFunction(u8"glGenerateMipmap");
	glFuncs.glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)extensionLoaderFunction(u8"glGenFramebuffers");
	glFuncs.glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)extensionLoaderFunction(u8"glGenRenderbuffers");
	glFuncs.glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)extensionLoaderFunction(u8"glGenVertexArrays");

	glFuncs.glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)extensionLoaderFunction(u8"glRenderbufferStorage");

	glFuncs.glUniform1ui = (PFNGLUNIFORM1UIPROC)extensionLoaderFunction(u8"glUniform1ui");
}
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
//Class header
#include <ACStdLib/Rendering/DeviceContext.hpp>
//Local
#include "OpenGL.h"
#include "CCubeMap.h"
#include "CFrameBuffer.h"
#include "CIndexBuffer.h"
#include "CInputState.h"
#include "CShader.h"
#include "CShaderProgram.h"
#include "CTexture2D.h"
#include "OpenGLVertexBuffer.hpp"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Rendering;
using namespace ACStdLib::UI;

//Global variables
uint32 g_currentTextureId = UINT32_MAX;

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

static inline uint32 MapTestFunction(ETestFunction func)
{
    switch(func)
    {
        case ETestFunction::Less:
            return GL_LESS;
        case ETestFunction::LessOrEqual:
            return GL_LEQUAL;
        case ETestFunction::NotEqual:
            return GL_NOTEQUAL;
        case ETestFunction::True:
            return GL_ALWAYS;
    }

    NOT_IMPLEMENTED_ERROR;
    return -1;
}

//Constructors
DeviceContext::DeviceContext(const RenderTargetWidget &refView)
{
    this->CreateOSContext(refView, 0);
    this->BindOSContext();

    glActiveTexture(GL_TEXTURE31); //see DeviceContext::SetTexture
}

DeviceContext::DeviceContext(const RenderTargetWidget &refView, uint8 nSamples)
{
    this->CreateOSContext(refView, nSamples);
    this->BindOSContext();

    glActiveTexture(GL_TEXTURE31); //see DeviceContext::SetTexture
}

//Destructor
DeviceContext::~DeviceContext()
{
    this->UnbindOSContext();

    this->DestroyOSContext();
}

//Public methods
void DeviceContext::ClearColorBuffer(const Color &refColor)
{
    this->BindOSContext();

    glClearColor(refColor.r, refColor.g, refColor.b, refColor.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void DeviceContext::ClearDepthBuffer()
{
    this->BindOSContext();

    glClear(GL_DEPTH_BUFFER_BIT);
}

ICubeMap *DeviceContext::CreateCubeMap()
{
    this->BindOSContext();

    glActiveTexture(GL_TEXTURE31);

    return new CCubeMap;
}

IFrameBuffer *DeviceContext::CreateFrameBuffer()
{
    this->BindOSContext();

    return new CFrameBuffer;
}

IIndexBuffer *DeviceContext::CreateIndexBuffer()
{
    this->BindOSContext();

    return new CIndexBuffer;
}

IInputState *DeviceContext::CreateInputState()
{
    this->BindOSContext();

    return new CInputState;
}

IShader *DeviceContext::CreateShader(IShader::EShaderType type)
{
    this->BindOSContext();

    return new CShader(type);
}

IShaderProgram *DeviceContext::CreateShaderProgram()
{
    this->BindOSContext();

    return new CShaderProgram;
}

ITexture2D *DeviceContext::CreateTexture2D()
{
    this->BindOSContext();

    glActiveTexture(GL_TEXTURE31);

    return new CTexture2D;
}

VertexBuffer *DeviceContext::CreateVertexBuffer()
{
    this->BindOSContext();

    return new OpenGLVertexBuffer;
}

void DeviceContext::DrawTriangleFan(uint32 startVertexIndex, uint32 nVertices)
{
    CInputState *pInputState;

    pInputState = (CInputState *)this->pCurrentInputState;

    //bind
    this->BindOSContext();
    pInputState->Bind();

    glDrawArrays(GL_TRIANGLE_FAN, startVertexIndex, nVertices);
}

void DeviceContext::DrawTriangles(uint32 startVertexIndex, uint32 nTriangles)
{
    CInputState *pInputState;

    pInputState = (CInputState *)this->pCurrentInputState;

    //bind
    this->BindOSContext();
    pInputState->Bind();

    glDrawArrays(GL_TRIANGLES, startVertexIndex, nTriangles * 3);
}

void DeviceContext::DrawTrianglesIndexed()
{
    CInputState *pInputState;
    CIndexBuffer *pIndexBuffer;

    this->BindOSContext();

    pInputState = (CInputState *)this->pCurrentInputState;
    pIndexBuffer = pInputState->GetIndexBuffer();

    pInputState->Bind();
    pIndexBuffer->Bind();

    if(pIndexBuffer->IndicesShort())
        glDrawElements(GL_TRIANGLES, pIndexBuffer->GetNumberOfIndices(), GL_UNSIGNED_SHORT, nullptr);
    else
        glDrawElements(GL_TRIANGLES, pIndexBuffer->GetNumberOfIndices(), GL_UNSIGNED_INT, nullptr);
}

void DeviceContext::DrawTriangleStrip(uint32 startVertexIndex, uint32 nVertices)
{
    CInputState *pInputState;

    pInputState = (CInputState *)this->pCurrentInputState;

    //bind
    this->BindOSContext();
    pInputState->Bind();

    glDrawArrays(GL_TRIANGLE_STRIP, startVertexIndex, nVertices);
}

void DeviceContext::EnableBlending(bool enabled)
{
    this->BindOSContext();

    if(enabled)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //TODO: this should be configureable, but mostly this is used, so its fine for now
    }
    else
        glDisable(GL_BLEND);
}

void DeviceContext::EnableColorBufferWriting(bool writeRed, bool writeGreen, bool writeBlue, bool writeAlpha)
{
    this->BindOSContext();

    glColorMask(writeRed, writeGreen, writeBlue, writeBlue);
}

void DeviceContext::EnableDepthBufferWriting(bool writeEnable)
{
    this->BindOSContext();

    glDepthMask(writeEnable);
}

void DeviceContext::EnableDepthTest(bool enabled)
{
    this->BindOSContext();

    if(enabled)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void DeviceContext::EnableFaceCulling(bool enabled)
{
    this->BindOSContext();

    if(enabled)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
}

void DeviceContext::EnablePolygonFilling(bool enabled)
{
    this->BindOSContext();

    if(enabled)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void DeviceContext::EnableStencilTest(bool enabled)
{
    this->BindOSContext();

    if(enabled)
        glEnable(GL_STENCIL_TEST);
    else
        glDisable(GL_STENCIL_TEST);
}

uint32 DeviceContext::GetNumberOfTextureUnits() const
{
    GLint maxTextures;

    this->BindOSContext();

    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextures);

    return maxTextures;
}

void DeviceContext::SetDepthTest(ETestFunction function)
{
    this->BindOSContext();

    glDepthFunc(MapTestFunction(function));
}

void DeviceContext::SetFrameBuffer(IFrameBuffer *pFrameBuffer)
{
    this->BindOSContext();

    if(pFrameBuffer == nullptr)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        CFrameBuffer::currentFBO = 0;
    }
    else
        ((CFrameBuffer *)pFrameBuffer)->Bind();
}

void DeviceContext::SetInputState(IInputState *pInputState)
{
    this->pCurrentInputState = pInputState;
}

void DeviceContext::SetProgram(IShaderProgram *pProgram)
{
    CShaderProgram *pShaderProgram;

    this->BindOSContext();

    pShaderProgram = (CShaderProgram *)pProgram;

    pShaderProgram->Use();
}

void DeviceContext::SetStencilTest(ETestFunction function, uint32 referenceValue, uint32 mask)
{
    this->BindOSContext();

    glStencilFunc(MapTestFunction(function), referenceValue, mask);
}

void DeviceContext::SetStencilTestEffects(ETestEffect stencilTestFailedEffect, ETestEffect stencilTestPassedDepthTestFailedEffect, ETestEffect stencilAndDepthTestsPassedEffect)
{
    this->BindOSContext();

    glStencilOp(MapTestEffect(stencilTestFailedEffect), MapTestEffect(stencilTestPassedDepthTestFailedEffect), MapTestEffect(stencilAndDepthTestsPassedEffect));
}

void DeviceContext::SetStencilTestEffects(bool frontFaces, ETestEffect stencilTestFailedEffect, ETestEffect stencilTestPassedDepthTestFailedEffect, ETestEffect stencilAndDepthTestsPassedEffect)
{
    this->BindOSContext();

    glStencilOpSeparate(frontFaces ? GL_FRONT : GL_BACK, MapTestEffect(stencilTestFailedEffect), MapTestEffect(stencilTestPassedDepthTestFailedEffect), MapTestEffect(stencilAndDepthTestsPassedEffect));
}

void DeviceContext::SetStencilWriteMask(uint32 mask)
{
    this->BindOSContext();

    glStencilMask(mask);
}

void DeviceContext::SetTexture(uint8 unit, ITexture *pTexture)
{
    this->BindOSContext();

    glActiveTexture(GL_TEXTURE0 + unit);

    switch(pTexture->GetType())
    {
        case ETextureType::CubeMap:
            glBindTexture(GL_TEXTURE_CUBE_MAP, ((CCubeMap *)pTexture)->GetId());
            break;
        case ETextureType::Texture_2D:
            glBindTexture(GL_TEXTURE_2D, ((CTexture2D *)pTexture)->GetId());
            break;
    }

    glActiveTexture(GL_TEXTURE31); //we use this as "dummy" texture, else textures get overwritten by others as soon as one "binds". Stupid open gl-.-
    //Only when DeviceContext::SetTexture is called glActiveTexture will call something different then GL_TEXTURE31 in order to set the texture
    //but then immediatly restores the dummy
}

void DeviceContext::SetViewPort(uint16 width, uint16 height)
{
    this->BindOSContext();

    glViewport(0, 0, width, height);
}
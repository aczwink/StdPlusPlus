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
//Class header
#include <Std++/Rendering/VectorPath/DevicePathRenderer.hpp>
//Local
#include <Std++/Rendering/InputLayout.hpp>
#include <Std++/Streams/BufferInputStream.hpp>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::Rendering;
//Definitions
#define LOCATION_UNIFORM_VIEWSIZE 0
#define LOCATION_UNIFORM_FRAGMENT_TYPE 1
#define LOCATION_UNIFORM_COLOR 2
#define LOCATION_UNIFORM_TEXTURE 3
#define LOCATION_UNIFORM_TRANSFORM 4

#define FRAGMENT_TYPE_COLOR 0
#define FRAGMENT_TYPE_TEXTURE 1

//Constructor
DevicePathRenderer::DevicePathRenderer(DeviceContext &refDC) : refDC(refDC)
{
	this->pShaderProgram = nullptr;
	this->pInputState = nullptr;
	this->pVertexBuffer = nullptr;

	this->pTexture = nullptr;

	//init rendering
	this->InitRendering();
}

//Destructor
DevicePathRenderer::~DevicePathRenderer()
{
	delete this->pVertexBuffer;
	delete this->pInputState;
	delete this->pShaderProgram;
}

//Public methods
void DevicePathRenderer::Sync()
{
	//now we render

	//update vertex buffer first
	//TODO: GL_STREAM_DRAW!!!
	this->pVertexBuffer->Allocate(this->vertices.GetNumberOfElements(), sizeof(SVertex), &this->vertices[0]);

	//prepare rendering
	this->pShaderProgram->SetUniformValue(LOCATION_UNIFORM_VIEWSIZE, this->viewSize);
	this->pShaderProgram->SetUniformValue(LOCATION_UNIFORM_TEXTURE, 0); //we always just need one texture unit
	this->refDC.SetInputState(this->pInputState);
	this->refDC.SetProgram(this->pShaderProgram);

	//render
	for(const SRenderEntry &refDrawCall : this->renderCalls)
	{
		this->pShaderProgram->SetUniformValue(LOCATION_UNIFORM_COLOR, refDrawCall.color);
		this->pShaderProgram->SetUniformValue(LOCATION_UNIFORM_TRANSFORM, this->state.transform); //TODO: this should be in the draw entry

		switch(refDrawCall.renderMethod)
		{
			case ERenderMethod::Fill:
			{
				this->refDC.EnableStencilTest();
				this->refDC.SetStencilWriteMask(0xFF);
				this->refDC.SetStencilTest(TestFunction::True, 0, 0xFF);
				this->refDC.EnableColorBufferWriting(false, false, false, false);
				this->refDC.SetStencilTestEffects(true, ETestEffect::KeepValue, ETestEffect::KeepValue, ETestEffect::IncrementWithWrapAround);
				this->refDC.SetStencilTestEffects(false, ETestEffect::KeepValue, ETestEffect::KeepValue, ETestEffect::DecrementWithWrapAround);

				for(const SPathAttributes &refPA : refDrawCall.pathAttributes)
					this->refDC.DrawTriangleFan(refPA.verticesBeginIndex, refPA.nVertices);

				this->refDC.EnableColorBufferWriting(true, true, true, true);
				this->refDC.SetStencilTest(TestFunction::NotEqual, 0, 0xFF);
				this->refDC.SetStencilTestEffects(ETestEffect::SetToZero, ETestEffect::SetToZero, ETestEffect::SetToZero);

				this->pShaderProgram->SetUniformValue(LOCATION_UNIFORM_FRAGMENT_TYPE, FRAGMENT_TYPE_COLOR);
				this->refDC.DrawTriangles(refDrawCall.boundingRectangleOffset, 2);

				this->refDC.EnableStencilTest(false);
			}
				break;
			case ERenderMethod::FillConvex:
			{
				if(refDrawCall.pTexture)
				{
					this->pShaderProgram->SetUniformValue(LOCATION_UNIFORM_FRAGMENT_TYPE, FRAGMENT_TYPE_TEXTURE);

					this->refDC.SetTexture(0, refDrawCall.pTexture);
				}
				else
					this->pShaderProgram->SetUniformValue(LOCATION_UNIFORM_FRAGMENT_TYPE, FRAGMENT_TYPE_COLOR);

				//only one path
				this->refDC.DrawTriangleFan(refDrawCall.pathAttributes[0].verticesBeginIndex, refDrawCall.pathAttributes[0].nVertices);
			}
				break;
			case ERenderMethod::Stroke:
			{
				this->pShaderProgram->SetUniformValue(LOCATION_UNIFORM_FRAGMENT_TYPE, FRAGMENT_TYPE_COLOR);

				for(const SPathAttributes &refPA : refDrawCall.pathAttributes)
					this->refDC.DrawTriangleStrip(refPA.verticesBeginIndex, refPA.nVertices);
			}
				break;
			default:
				NOT_IMPLEMENTED_ERROR;
		}
	}

	//throw out what is not needed anymore
	this->vertices.Clear();
	this->renderCalls.Release();
}

//Private methods
void DevicePathRenderer::InitRendering()
{
	bool result;
	Shader *pVSShader, *pFSShader;

	static const char vsShaderCode[] =
		{
			"#version 330\r\n"
				"#extension GL_ARB_explicit_uniform_location : require\r\n"

				"layout(location = 0) uniform vec2 viewSize;\r\n"

				"layout(location = 0) in vec2 pos;\r\n"

				"out vec2 textureCoords;\r\n"

				"void main(void)\r\n"
				"{\r\n"
				"gl_Position = vec4(2.0 * pos.x / viewSize.x - 1.0, 2.0 * pos.y / viewSize.y - 1.0, 0, 1);\r\n"
				"textureCoords = pos / viewSize;\r\n"
				"}\r\n"
		};

	static const char fsShaderCode[] =
		{
			"#version 150\r\n"
				"#extension GL_ARB_explicit_uniform_location : require\r\n"

				"layout(location = 1) uniform int fragmentType;\r\n"
				"layout(location = 2) uniform vec4 color;\r\n"
				"layout(location = 3) uniform sampler2D textureSampler;\r\n"
				"layout(location = 4) uniform mat2 transform;"

				"in vec2 textureCoords;\r\n"

				"out vec4 outColor;\r\n"

				"void main(void)\r\n"
				"{\r\n"

				"if(fragmentType == 0)\r\n"
				"{\r\n"
				"outColor = color;\r\n"
				"}\r\n"
				"else if(fragmentType == 1)\r\n"
				"{\r\n"
				"outColor = texture(textureSampler, transform * textureCoords);\r\n"
				"}\r\n"

				"}\r\n"
		};

	//compile vertex shader
	pVSShader = refDC.CreateShader(Shader::ShaderType::VertexShader);
	BufferInputStream vsInput((byte *)vsShaderCode, sizeof(vsShaderCode));
	result = pVSShader->Compile(vsInput);
	ASSERT(result, pVSShader->GetCompilationLog());

	//compile fragment shader
	pFSShader = refDC.CreateShader(Shader::ShaderType::FragmentShader);
	BufferInputStream fsInput((byte *)fsShaderCode, sizeof(fsShaderCode));
	result = pFSShader->Compile(fsInput);
	ASSERT(result, pFSShader->GetCompilationLog());

	//create program
	this->pShaderProgram = refDC.CreateShaderProgram();

	this->pShaderProgram->AttachShader(pVSShader);
	this->pShaderProgram->AttachShader(pFSShader);
	this->pShaderProgram->Link();
	this->pShaderProgram->DetachShader(pVSShader);
	this->pShaderProgram->DetachShader(pFSShader);

	delete pVSShader;
	delete pFSShader;

	//init vertex buffer
	this->pVertexBuffer = refDC.CreateVertexBuffer();

	//create input layout
	InputLayout inputLayout;

	inputLayout.AddAttribute2(); //pos

	//init input state
	this->pInputState = refDC.CreateInputState();

	this->pInputState->AddVertexBuffer(this->pVertexBuffer, inputLayout);
}

void DevicePathRenderer::RenderFill(const DynamicArray<FlatVectorPath *> &refPaths, DynamicArray<SPathAttributes> &&refPathAttributes, vec2f64 boundingRectMin, vec2f64 boundingRectMax)
{
	SRenderEntry entry;

	//fill out entry
	entry.renderMethod = ERenderMethod::Fill;
	if(refPaths.GetNumberOfElements() == 1 && refPaths[0]->IsConvex())
		entry.renderMethod = ERenderMethod::FillConvex;

	entry.pathAttributes = refPathAttributes;

	entry.color = this->state.fillColor;
	entry.pTexture = this->pTexture;

	//in case we have a normal fill we need also a quad of the bounding rectangle
	if(entry.renderMethod == ERenderMethod::Fill)
	{
		entry.boundingRectangleOffset = this->vertices.GetNumberOfElements();

		this->AddVertex(boundingRectMin.x, boundingRectMax.y);
		this->AddVertex(boundingRectMax.x, boundingRectMax.y);
		this->AddVertex(boundingRectMax.x, boundingRectMin.y);

		this->AddVertex(boundingRectMin.x, boundingRectMax.y);
		this->AddVertex(boundingRectMax.x, boundingRectMin.y);
		this->AddVertex(boundingRectMin.x, boundingRectMin.y);
	}

	//put render entry
	this->renderCalls.InsertTail(entry);
}

void DevicePathRenderer::RenderStroke(const DynamicArray<FlatVectorPath *> &refPaths, DynamicArray<SPathAttributes> &&refPathAttributes)
{
	SRenderEntry entry;

	entry.renderMethod = ERenderMethod::Stroke;

	entry.pathAttributes = refPathAttributes;

	entry.color = this->state.strokeColor;

	//put render entry
	this->renderCalls.InsertTail(entry);
}
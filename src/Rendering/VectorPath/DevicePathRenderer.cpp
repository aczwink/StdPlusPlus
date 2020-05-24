/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
using namespace StdXX;
using namespace StdXX::Rendering;
//Definitions
#define FRAGMENT_TYPE_COLOR 0
#define FRAGMENT_TYPE_TEXTURE 1

//Constructor
DevicePathRenderer::DevicePathRenderer(DeviceContext &refDC) : refDC(refDC)
{
	this->shaderProgram = nullptr;
	this->pInputState = nullptr;
	this->pVertexBuffer = nullptr;

	this->texture = nullptr;

	//init rendering
	this->InitRendering();
}

//Destructor
DevicePathRenderer::~DevicePathRenderer()
{
	delete this->pVertexBuffer;
	delete this->pInputState;
	delete this->shaderProgram;
}

//Public methods
void DevicePathRenderer::Sync()
{
	//now we render

	//update vertex buffer first
	//TODO: GL_STREAM_DRAW!!!
	this->pVertexBuffer->Allocate(this->vertices.GetNumberOfElements(), sizeof(Vertex), &this->vertices[0]);

	//prepare rendering
	this->shaderProgram->SetUniformValue(this->uniformLocations.textureSampler, 0); //we always just need one texture unit
	this->refDC.SetInputState(this->pInputState);
	this->refDC.SetProgram(this->shaderProgram);

	Math::Vector3S scale = { 2 / this->viewSize.x, 2 / this->viewSize.y, 1 };
	Math::Matrix3S transform = Math::Matrix3S::Translate({-1, -1}) * Math::Matrix3S::Scale(scale) * this->state.transform;

	//render
	for(const RenderEntry& drawCall : this->renderCalls)
	{
		this->shaderProgram->SetUniformValue(this->uniformLocations.color, drawCall.color);
		this->shaderProgram->SetUniformValue(this->uniformLocations.transform, transform); //TODO: this should be in the draw entry

		scale = Math::Vector3S(1.0 / (drawCall.boundingRectMax - drawCall.boundingRectMin).x, 1.0 / (drawCall.boundingRectMax - drawCall.boundingRectMin).y, 1);
		Math::Vector2S translate = { static_cast<float32>(-drawCall.boundingRectMin.x), static_cast<float32>(-drawCall.boundingRectMin.y) };
		Math::Matrix3S uvTransform = Math::Matrix3S::Scale(scale) * Math::Matrix3S::Translate(translate);

		this->shaderProgram->SetUniformValue(this->uniformLocations.uvTransform, uvTransform);

		switch(drawCall.renderMethod)
		{
			case RenderMethod::Fill:
			{
				this->refDC.EnableStencilTest();
				this->refDC.SetStencilWriteMask(0xFF);
				this->refDC.SetStencilTest(TestFunction::True, 0, 0xFF);
				this->refDC.EnableColorBufferWriting(false, false, false, false);
				this->refDC.SetStencilTestEffects(true, ETestEffect::KeepValue, ETestEffect::KeepValue, ETestEffect::IncrementWithWrapAround);
				this->refDC.SetStencilTestEffects(false, ETestEffect::KeepValue, ETestEffect::KeepValue, ETestEffect::DecrementWithWrapAround);

				for(const PathAttributes &refPA : drawCall.pathAttributes)
					this->refDC.DrawTriangleFan(refPA.verticesBeginIndex, refPA.nVertices);

				this->refDC.EnableColorBufferWriting(true, true, true, true);
				this->refDC.SetStencilTest(TestFunction::NotEqual, 0, 0xFF);
				this->refDC.SetStencilTestEffects(ETestEffect::SetToZero, ETestEffect::SetToZero, ETestEffect::SetToZero);

				this->shaderProgram->SetUniformValue(this->uniformLocations.fragmentType, FRAGMENT_TYPE_COLOR);
				this->refDC.DrawTriangles(drawCall.boundingRectangleOffset, 2);

				this->refDC.EnableStencilTest(false);
			}
				break;
			case RenderMethod::FillConvex:
			{
				if(drawCall.texture)
				{
					this->shaderProgram->SetUniformValue(this->uniformLocations.fragmentType, FRAGMENT_TYPE_TEXTURE);

					this->refDC.SetTexture(0, drawCall.texture);
				}
				else
					this->shaderProgram->SetUniformValue(this->uniformLocations.fragmentType, FRAGMENT_TYPE_COLOR);

				//only one path
				this->refDC.DrawTriangleFan(drawCall.pathAttributes[0].verticesBeginIndex, drawCall.pathAttributes[0].nVertices);
			}
				break;
			case RenderMethod::Stroke:
			{
				this->shaderProgram->SetUniformValue(this->uniformLocations.fragmentType, FRAGMENT_TYPE_COLOR);

				for(const PathAttributes &refPA : drawCall.pathAttributes)
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
		"#extension GL_ARB_explicit_attrib_location : require\r\n"
		"uniform mat3 transform;"
		"uniform mat3 uvTransform;"
		"layout(location = 0) in vec2 pos;\r\n"
		"out vec2 textureCoords;\r\n"
		"void main(void)\r\n"
		"{\r\n"
  		"vec3 transformed = transform * vec3(pos, 1);"
		"gl_Position = vec4(transformed, 1);\r\n"
		"textureCoords = (uvTransform * vec3(pos, 1)).xy;\r\n"
		"}\r\n"
		};

	static const char fsShaderCode[] =
		{
		"#version 150\r\n"
		"#extension GL_ARB_explicit_attrib_location : require\r\n"
		
		"uniform int fragmentType;\r\n"
		"uniform vec4 color;\r\n"
		"uniform sampler2D textureSampler;\r\n"
		
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
		"outColor = texture(textureSampler, textureCoords);\r\n"
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
	this->shaderProgram = refDC.CreateShaderProgram();

	this->shaderProgram->AttachShader(pVSShader);
	this->shaderProgram->AttachShader(pFSShader);
	this->shaderProgram->Link();
	this->shaderProgram->DetachShader(pVSShader);
	this->shaderProgram->DetachShader(pFSShader);

	delete pVSShader;
	delete pFSShader;

	//get uniform locations
	this->uniformLocations.color = this->shaderProgram->GetUniformId(u8"color");
	this->uniformLocations.fragmentType = this->shaderProgram->GetUniformId(u8"fragmentType");
	this->uniformLocations.textureSampler = this->shaderProgram->GetUniformId(u8"textureSampler");
	this->uniformLocations.transform = this->shaderProgram->GetUniformId(u8"transform");
	this->uniformLocations.uvTransform = this->shaderProgram->GetUniformId(u8"uvTransform");

	//init vertex buffer
	this->pVertexBuffer = refDC.CreateVertexBuffer();

	//create input layout
	InputLayout inputLayout;

	inputLayout.AddAttribute2(); //pos

	//init input state
	this->pInputState = refDC.CreateInputState();

	this->pInputState->AddVertexBuffer(this->pVertexBuffer, inputLayout);
}

void DevicePathRenderer::RenderFill(const DynamicArray<FlatVectorPath *> &refPaths, DynamicArray<PathAttributes> &&pathAttributes, Math::Vector2D boundingRectMin, Math::Vector2D boundingRectMax)
{
	RenderEntry entry;

	//fill out entry
	entry.renderMethod = RenderMethod::Fill;
	if(refPaths.GetNumberOfElements() == 1 && refPaths[0]->IsConvex())
		entry.renderMethod = RenderMethod::FillConvex;

	entry.color = this->state.fillColor;
	entry.texture = this->texture;
	entry.pathAttributes = Move(pathAttributes);
	entry.boundingRectMin = boundingRectMin;
	entry.boundingRectMax = boundingRectMax;

	//in case we have a normal fill we need also a quad of the bounding rectangle
	if(entry.renderMethod == RenderMethod::Fill)
	{
		entry.boundingRectangleOffset = this->vertices.GetNumberOfElements();

		this->AddVertex(boundingRectMin.x, boundingRectMax.y, 0, 1);
		this->AddVertex(boundingRectMax.x, boundingRectMax.y, 1, 1);
		this->AddVertex(boundingRectMax.x, boundingRectMin.y, 1, 0);

		this->AddVertex(boundingRectMin.x, boundingRectMax.y, 0, 1);
		this->AddVertex(boundingRectMax.x, boundingRectMin.y, 1, 0);
		this->AddVertex(boundingRectMin.x, boundingRectMin.y, 0, 0);
	}

	//put render entry
	this->renderCalls.InsertTail(entry);
}

void DevicePathRenderer::RenderStroke(const DynamicArray<FlatVectorPath *> &refPaths, DynamicArray<PathAttributes> &&refPathAttributes)
{
	RenderEntry entry;

	entry.renderMethod = RenderMethod::Stroke;

	entry.pathAttributes = refPathAttributes;

	entry.color = this->state.strokeColor;

	//put render entry
	this->renderCalls.InsertTail(entry);
}
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
#include "OpenGLInputState.hpp"
//Local
#include <Std++/Rendering/InputLayout.hpp>
#include "OpenGLIndexBuffer.hpp"
#include "OpenGLVertexBuffer.hpp"

//Constructor
OpenGLInputState::OpenGLInputState(OpenGLDeviceContext &deviceContext) : deviceContext(deviceContext), glFuncs(deviceContext.glFuncs)
{
    this->glFuncs.glGenVertexArrays(1, &this->id);

    this->currentAttributeIndex = 0;
}

//Destructor
OpenGLInputState::~OpenGLInputState()
{
    this->glFuncs.glDeleteVertexArrays(1, &this->id);
}

//Public methods
void OpenGLInputState::AddVertexBuffer(const VertexBuffer *vertexBuffer, const InputLayout &inputLayout)
{
    this->Bind();
    OpenGLVertexBuffer *openGLVertexBuffer = (OpenGLVertexBuffer *)vertexBuffer;
    openGLVertexBuffer->Bind();

    for(uint8 i = 0; i < inputLayout.GetNumberOfAttributes(); i++)
    {
        auto &refVA = inputLayout.GetAttribute(i);

        uint8 location = this->currentAttributeIndex + i;

        this->glFuncs.glEnableVertexAttribArray(location);
        this->glFuncs.glVertexAttribPointer(location, refVA.nComponents, GL_FLOAT, false, inputLayout.GetSize(), (void *)refVA.offset);
    }

    this->currentAttributeIndex += inputLayout.GetNumberOfAttributes();
}

void OpenGLInputState::SetIndexBuffer(const IndexBuffer *indexBuffer)
{
    this->indexBuffer = dynamic_cast<const OpenGLIndexBuffer *>(indexBuffer);
}
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
#include "../../src_backends/OpenGL3Core/Rendering/OpenGLInputState.hpp"
//Local
#include <Std++/Rendering/InputLayout.hpp>
#include "../../src_backends/OpenGL3Core/Rendering/OpenGLIndexBuffer.h"
#include "../../src_backends/OpenGL3Core/Rendering/OpenGLVertexBuffer.hpp"

//Constructor
OpenGLInputState::OpenGLInputState()
{
    glGenVertexArrays(1, &this->id);

    this->currentAttributeIndex = 0;
}

//Destructor
OpenGLInputState::~OpenGLInputState()
{
    glDeleteVertexArrays(1, &this->id);
}

//Public methods
void OpenGLInputState::AddVertexBuffer(VertexBuffer *pVB, const InputLayout &refInputLayout)
{
    uint8 i, location;
    OpenGLVertexBuffer *pVertexBuffer;

    pVertexBuffer = (OpenGLVertexBuffer *)pVB;

    this->Bind();
    pVertexBuffer->Bind();

    for(i = 0; i < refInputLayout.GetNumberOfAttributes(); i++)
    {
        auto &refVA = refInputLayout.GetAttribute(i);

        location = this->currentAttributeIndex + i;

        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, refVA.nComponents, GL_FLOAT, false, refInputLayout.GetSize(), (void *)refVA.offset);
    }

    this->currentAttributeIndex += refInputLayout.GetNumberOfAttributes();
}

void OpenGLInputState::SetIndexBuffer(IndexBuffer *pIndexBuffer)
{
    this->pIndexBuffer = (OpenGLIndexBuffer *)pIndexBuffer;
}
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
#include "OpenGLIndexBuffer.h"
//Local
#include "Shared.hpp"

//Constructor
OpenGLIndexBuffer::OpenGLIndexBuffer(AllocationPolicy policy)
{
    glGenBuffers(1, &this->id);
    this->policy = policy;
}

//Destructor
OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    glDeleteBuffers(1, &this->id);
}

//Public methods
void OpenGLIndexBuffer::Allocate(uint32 nIndices, const uint16 *pIndices)
{
    this->Bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(*pIndices), pIndices, AllocationPolicyToGL(this->policy));

    this->nIndices = nIndices;
    this->isShort = true;
}

void OpenGLIndexBuffer::Allocate(uint32 nIndices, const uint32 *pIndices)
{
    this->Bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(*pIndices), pIndices, AllocationPolicyToGL(this->policy));

    this->nIndices = nIndices;
    this->isShort = false;
}
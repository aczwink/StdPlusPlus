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
#include "OpenGLVertexBuffer.hpp"
//Local
#include "Shared.hpp"

//Constructor
OpenGLVertexBuffer::OpenGLVertexBuffer(AllocationPolicy policy)
{
    glGenBuffers(1, &this->id);
	this->policy = policy;
}

//Destructor
OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    glDeleteBuffers(1, &this->id);
}

//Public methods
void OpenGLVertexBuffer::Allocate(uint32 nVertices, uint32 vertexSize, const void *pData)
{
    this->Bind();

    glBufferData(GL_ARRAY_BUFFER, nVertices * vertexSize, pData, AllocationPolicyToGL(this->policy));
}

void OpenGLVertexBuffer::Write(uint32 offset, uint32 size, const void *data)
{
	this->Bind();

	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
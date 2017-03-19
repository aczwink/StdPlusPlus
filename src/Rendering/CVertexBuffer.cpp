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
#include "CVertexBuffer.h"

//Constructor
CVertexBuffer::CVertexBuffer()
{
    glGenBuffers(1, &this->id);
}

//Destructor
CVertexBuffer::~CVertexBuffer()
{
    glDeleteBuffers(1, &this->id);
}

//Public methods
void CVertexBuffer::Allocate(uint32 nVertices, uint32 vertexSize, const void *pData)
{
    this->Bind();

    glBufferData(GL_ARRAY_BUFFER, nVertices * vertexSize, pData, GL_STATIC_DRAW);
}
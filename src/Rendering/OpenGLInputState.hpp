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
#pragma once
//Local
#include <Std++/Rendering/InputState.hpp>
#include "OpenGL.h"
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::Rendering;

//Move declarations
class OpenGLIndexBuffer;

class OpenGLInputState : public InputState
{
private:
    //Members
    uint32 id;
    uint8 currentAttributeIndex;
    OpenGLIndexBuffer *pIndexBuffer;

public:
    //Constructor
    OpenGLInputState();

    //Destructor
    ~OpenGLInputState();

    //Methods
    void AddVertexBuffer(VertexBuffer *pVertexBuffer, const InputLayout &refInputLayout);
    void SetIndexBuffer(IndexBuffer *pIndexBuffer);

    //Inline
    inline void Bind() const
    {
        glBindVertexArray(this->id);
    }

    inline OpenGLIndexBuffer *GetIndexBuffer()
    {
        return this->pIndexBuffer;
    }
};
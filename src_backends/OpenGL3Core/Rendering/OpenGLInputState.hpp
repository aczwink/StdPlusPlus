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
#include "../GL3Functions.hpp"
#include "OpenGLDeviceContext.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Rendering;

//Forward declarations
class OpenGLIndexBuffer;

class OpenGLInputState : public InputState
{
public:
    //Constructor
    OpenGLInputState(OpenGLDeviceContext &deviceContext);

    //Destructor
    ~OpenGLInputState();

    //Methods
    void AddVertexBuffer(const VertexBuffer *vertexBuffer, const InputLayout &inputLayout);
    void SetIndexBuffer(const IndexBuffer *indexBuffer);

    //Inline
    inline void Bind() const
    {
        this->glFuncs.glBindVertexArray(this->id);
    }

    inline const OpenGLIndexBuffer *GetIndexBuffer() const
    {
        return this->indexBuffer;
    }
private:
	//Members
	OpenGLDeviceContext &deviceContext;
    GLFunctions_3_0 &glFuncs;
	uint32 id;
	uint8 currentAttributeIndex;
	const OpenGLIndexBuffer *indexBuffer;
};
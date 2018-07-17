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
#include <Std++/Rendering/DeviceContext.hpp>
#include <Std++/Rendering/IndexBuffer.hpp>
#include "../GL3Functions.hpp"
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::Rendering;

class OpenGLIndexBuffer : public IndexBuffer
{
private:
    //Members
    uint32 id;
    uint32 nIndices;
    bool isShort;
	AllocationPolicy policy;

public:
    //Constructor
    OpenGLIndexBuffer(AllocationPolicy policy);

    //Destructor
    ~OpenGLIndexBuffer();

    //Methods
    void Allocate(uint32 nIndices, const uint16 *pIndices);
    void Allocate(uint32 nIndices, const uint32 *pIndices);

    //Inline
    inline void Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
    }

    inline uint32 GetNumberOfIndices() const
    {
        return this->nIndices;
    }

    inline bool IndicesShort() const
    {
        return this->isShort;
    }
};
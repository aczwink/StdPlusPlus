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
#pragma once
//Local
#include <ACStdLib/Rendering/IIndexBuffer.h>
#include "OpenGL.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Rendering;

class CIndexBuffer : public IIndexBuffer
{
private:
    //Members
    uint32 id;
    uint32 nIndices;
    bool isShort;

public:
    //Constructor
    CIndexBuffer();

    //Destructor
    ~CIndexBuffer();

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
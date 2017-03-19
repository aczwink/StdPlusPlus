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
#include "CInputState.h"
//Local
#include "../../headers/Rendering/CInputLayout.h"
#include "CIndexBuffer.h"
#include "CVertexBuffer.h"

//Constructor
CInputState::CInputState()
{
    glGenVertexArrays(1, &this->id);

    this->currentAttributeIndex = 0;
}

//Destructor
CInputState::~CInputState()
{
    glDeleteVertexArrays(1, &this->id);
}

//Public methods
void CInputState::AddVertexBuffer(IVertexBuffer *pVB, const CInputLayout &refInputLayout)
{
    uint8 i, location;
    CVertexBuffer *pVertexBuffer;

    pVertexBuffer = (CVertexBuffer *)pVB;

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

void CInputState::SetIndexBuffer(IIndexBuffer *pIndexBuffer)
{
    this->pIndexBuffer = (CIndexBuffer *)pIndexBuffer;
}
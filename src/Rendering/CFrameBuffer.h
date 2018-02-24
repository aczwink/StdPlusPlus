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
#include <Std++/Rendering/IFrameBuffer.h>
#include <Std++/Rendering/DeviceContext.hpp>
#include "OpenGL.h"
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::Rendering;

class CFrameBuffer : public IFrameBuffer
{
    friend class StdPlusPlus::Rendering::DeviceContext;
private:
    //Members
    uint32 id;

    //Static fields
    static uint32 currentFBO;

    //Inline
    inline void Bind() const
    {
        if(currentFBO != this->id)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, this->id);
            currentFBO = this->id;
        }
    }

public:
    //Constructor
    inline CFrameBuffer()
    {
        glGenFramebuffers(1, &this->id);
    }

    //Destructor
    ~CFrameBuffer();

    //Methods
    uint32 GetStatus() const;
    void SetColorBuffer(Texture2D *pTexture);
    void SetDepthBuffer(Texture2D *pTexture);
};
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
#include "CFrameBuffer.h"
//Local
#include "CTexture2D.h"

//Static fields
uint32 CFrameBuffer::currentFBO;

//Destructor
CFrameBuffer::~CFrameBuffer()
{
    glDeleteFramebuffers(1, &this->id);
}

//Public methods
uint32 CFrameBuffer::GetStatus() const
{
    this->Bind();

    return glCheckFramebufferStatus(GL_FRAMEBUFFER);
}

void CFrameBuffer::SetColorBuffer(ITexture2D *pTexture)
{
    this->Bind();

    if(pTexture)
    {
        CTexture2D *pGL_Texture;

        pGL_Texture = (CTexture2D *)pTexture;

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pGL_Texture->GetId(), 0);
    }
    else
    {
        //dont use any color buffer
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
}

void CFrameBuffer::SetDepthBuffer(ITexture2D *pTexture)
{
    if(pTexture)
    {
        CTexture2D *pGL_Texture;

        pGL_Texture = (CTexture2D *)pTexture;

        this->Bind();

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, pGL_Texture->GetId(), 0);
    }
    else
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
    }
}
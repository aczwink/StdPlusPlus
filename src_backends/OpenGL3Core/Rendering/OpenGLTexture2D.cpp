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
#include "OpenGLTexture2D.hpp"

//Constructor
OpenGLTexture2D::OpenGLTexture2D(OpenGLDeviceContext &deviceContext) : deviceContext(deviceContext), glFuncs(deviceContext.glFuncs)
{
    this->glFuncs.glGenTextures(1, &this->id);

    this->Bind();

    //what to do on UV overlap
    this->glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    this->glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

//Destructor
OpenGLTexture2D::~OpenGLTexture2D()
{
    this->glFuncs.glDeleteTextures(1, &this->id);
}

//Public methods
void OpenGLTexture2D::AllocateDepth(uint16 width, uint16 height)
{
    this->Bind();

    this->glFuncs.glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    //action when texture is scaled
    this->glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    this->glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void OpenGLTexture2D::AllocateDXT1(uint16 width, uint16 height, const void *pCompressedData)
{
    this->Bind();

    this->glFuncs.glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, width, height, 0, width * height * 8 / 16, pCompressedData);

    //action when texture is scaled
    this->glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    this->glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void OpenGLTexture2D::AllocateDXT5(uint16 width, uint16 height, const void *pCompressedData)
{
    this->Bind();

    this->glFuncs.glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, width, height, 0, width * height * 16 / 16, pCompressedData);

    //action when texture is scaled
    this->glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    this->glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void OpenGLTexture2D::AllocateRGB(const Math::Size<uint16> &size, const void *pColorData)
{
    this->Bind();

    this->glFuncs.glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, size.width, size.height, 0, GL_RGB, GL_UNSIGNED_BYTE, pColorData);

    //action when texture is scaled
    this->glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    this->glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void OpenGLTexture2D::AllocateRGBA(uint16 width, uint16 height, const void *pColorData)
{
    this->Bind();

    this->glFuncs.glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pColorData);

    //action when texture is scaled
    this->glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    this->glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void OpenGLTexture2D::GenerateMipMaps()
{
    this->Bind();

    this->glFuncs.glGenerateMipmap(GL_TEXTURE_2D);
}

ETextureType OpenGLTexture2D::GetType() const
{
    return ETextureType::Texture_2D;
}

void OpenGLTexture2D::SetMaximumMipMapLevel(uint16 level)
{
    this->Bind();

    this->glFuncs.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, level);
}

void OpenGLTexture2D::UpdateRGB(uint16 offsetX, uint16 offsetY, uint16 width, uint16 height, const Vector3s *pColorData)
{
    this->Bind();

    this->glFuncs.glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, width, height, GL_RGB, GL_FLOAT, pColorData);
}
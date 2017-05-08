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
#include "CTexture2D.h"

//Constructor
CTexture2D::CTexture2D()
{
    glGenTextures(1, &this->id);

    this->Bind();

    //what to do on UV overlap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

//Destructor
CTexture2D::~CTexture2D()
{
    glDeleteTextures(1, &this->id);
}

//Public methods
void CTexture2D::AllocateDepth(uint16 width, uint16 height)
{
    this->Bind();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    //action when texture is scaled
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void CTexture2D::AllocateDXT1(uint16 width, uint16 height, const void *pCompressedData)
{
    this->Bind();

    glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, width, height, 0, width * height * 8 / 16, pCompressedData);

    //action when texture is scaled
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void CTexture2D::AllocateDXT5(uint16 width, uint16 height, const void *pCompressedData)
{
    this->Bind();

    glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, width, height, 0, width * height * 16 / 16, pCompressedData);

    //action when texture is scaled
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void CTexture2D::AllocateRGB(uint16 width, uint16 height, const void *pColorData)
{
    this->Bind();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pColorData);

    //action when texture is scaled
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void CTexture2D::AllocateRGBA(uint16 width, uint16 height, const void *pColorData)
{
    this->Bind();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pColorData);

    //action when texture is scaled
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void CTexture2D::GenerateMipMaps()
{
    this->Bind();

    glGenerateMipmap(GL_TEXTURE_2D);
}

ETextureType CTexture2D::GetType() const
{
    return ETextureType::Texture_2D;
}

void CTexture2D::SetMaximumMipMapLevel(uint16 level)
{
    this->Bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, level);
}

void CTexture2D::UpdateRGB(uint16 offsetX, uint16 offsetY, uint16 width, uint16 height, const Vector3 *pColorData)
{
    this->Bind();

    glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, width, height, GL_RGB, GL_FLOAT, pColorData);
}
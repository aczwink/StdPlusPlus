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
#include "../../src_backends/OpenGL3Core/Rendering/OpenGLCubeMap.hpp"

//Constructor
OpenGLCubeMap::OpenGLCubeMap()
{
    glGenTextures(1, &this->id);

    this->Bind();

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

//Destructor
OpenGLCubeMap::~OpenGLCubeMap()
{
    glDeleteTextures(1, &this->id);
}

//Public methods
ETextureType OpenGLCubeMap::GetType() const
{
    return ETextureType::CubeMap;
}

void OpenGLCubeMap::SetBackTexture(uint16 width, uint16 height, const void *pCompressedData)
{
    this->Bind();

    //z is inverted => LHS coordinate system

    glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, width, height, 0, width * height * 8 / 16, pCompressedData);
}

void OpenGLCubeMap::SetBottomTexture(uint16 width, uint16 height, const void *pCompressedData)
{
    this->Bind();

    glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, width, height, 0, width * height * 8 / 16, pCompressedData);
}

void OpenGLCubeMap::SetFrontTexture(uint16 width, uint16 height, const void *pCompressedData)
{
    this->Bind();

    //z is inverted => LHS coordinate system

    glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, width, height, 0, width * height * 8 / 16, pCompressedData);
}

void OpenGLCubeMap::SetLeftTexture(uint16 width, uint16 height, const void *pCompressedData)
{
    this->Bind();

    glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, width, height, 0, width * height * 8 / 16, pCompressedData);
}

void OpenGLCubeMap::SetRightTexture(uint16 width, uint16 height, const void *pCompressedData)
{
    this->Bind();

    glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, width, height, 0, width * height * 8 / 16, pCompressedData);
}

void OpenGLCubeMap::SetTopTexture(uint16 width, uint16 height, const void *pCompressedData)
{
    this->Bind();

    glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, width, height, 0, width * height * 8 / 16, pCompressedData);
}
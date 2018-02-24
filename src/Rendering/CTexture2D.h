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
#include <Std++/Rendering/Texture2D.hpp>
#include "OpenGL.h"
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::Math;
using namespace StdPlusPlus::Rendering;

//Global variables
extern uint32 g_currentTextureId;

class CTexture2D : public Texture2D
{
    friend class DeviceContext;
private:
    //Members
    uint32 id;

    //Inline
    inline void Bind()
    {
        if(g_currentTextureId != this->id)
        {
            glBindTexture(GL_TEXTURE_2D, this->id);
            g_currentTextureId = this->id;
        }
    }

public:
    //Constructor
    CTexture2D();

    //Destructor
    ~CTexture2D();

    //Methods
    void AllocateDepth(uint16 width, uint16 height);
    void AllocateDXT1(uint16 width, uint16 height, const void *pCompressedData);
    void AllocateDXT5(uint16 width, uint16 height, const void *pCompressedData);
    void AllocateRGB(const Size &size, const void *pColorData);
    void AllocateRGBA(uint16 width, uint16 height, const void *pColorData);
    void GenerateMipMaps();
    ETextureType GetType() const;
    void SetMaximumMipMapLevel(uint16 level);
    void UpdateRGB(uint16 offsetX, uint16 offsetY, uint16 width, uint16 height, const Vector3 *pColorData);

    //Inline
    inline uint32 GetId() const
    {
        return this->id;
    }
};
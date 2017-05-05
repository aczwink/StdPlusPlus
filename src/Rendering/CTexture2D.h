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
#include <ACStdLib/Rendering/ITexture2D.h>
#include "OpenGL.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;
using namespace ACStdLib::Rendering;

//Global variables
extern uint32 g_currentTextureId;

class CTexture2D : public ITexture2D
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
    void AllocateRGB(uint16 width, uint16 height, const void *pColorData);
    void AllocateRGBA(uint16 width, uint16 height, const void *pColorData);
    void GenerateMipMaps();
    ETextureType GetType() const;
    void SetMaximumMipMapLevel(uint16 level);
    void UpdateRGB(uint16 offsetX, uint16 offsetY, uint16 width, uint16 height, const CVector3 *pColorData);

    //Inline
    inline uint32 GetId() const
    {
        return this->id;
    }
};
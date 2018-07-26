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
#include "../Math/Vector3.hpp"
#include "../Math/Size.hpp"
#include "ITexture.h"

namespace StdXX
{
    namespace Rendering
    {
        class STDPLUSPLUS_API Texture2D : public ITexture
        {
        public:
            //Abstract
            virtual void AllocateDepth(uint16 width, uint16 height) = 0;
            virtual void AllocateDXT1(uint16 width, uint16 height, const void *pCompressedData) = 0;
            virtual void AllocateDXT5(uint16 width, uint16 height, const void *pCompressedData) = 0;
            virtual void AllocateRGB(const Math::Size<uint16> &size, const void *pColorData) = 0;
            virtual void AllocateRGBA(uint16 width, uint16 height, const void *pColorData) = 0;
            virtual void GenerateMipMaps() = 0;
            virtual void SetMaximumMipMapLevel(uint16 level) = 0;
            virtual void UpdateRGB(uint16 offsetX, uint16 offsetY, uint16 width, uint16 height, const Math::Vector3s *pColorData) = 0;
        };
    }
}
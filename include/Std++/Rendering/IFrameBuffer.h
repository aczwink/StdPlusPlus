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
#include "../Definitions.h"
#include "Texture2D.hpp"

namespace StdPlusPlus
{
    namespace Rendering
    {
        class STDPLUSPLUS_API IFrameBuffer
        {
        public:
            //Destructor
            virtual ~IFrameBuffer() {}

            //Abstract
            virtual uint32 GetStatus() const = 0; //TODO: only for debugging... throw it out
            virtual void SetColorBuffer(Texture2D *pTexture) = 0;
            virtual void SetDepthBuffer(Texture2D *pTexture) = 0;
        };
    }
}
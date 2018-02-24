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
#include <Std++/Definitions.h>
#include <Std++/Math/Size.hpp>
#include <Std++/UI/ERenderMode.h>

namespace StdPlusPlus
{
    namespace UI
    {
        class IRenderTarget
        {
        public:
            virtual ERenderMode GetRenderMode() const = 0;
            virtual Size GetSize() const = 0;
            virtual void Present() = 0;
        };

        //Functions
        IRenderTarget &GetRenderTarget(ERenderMode mode);
    }
}
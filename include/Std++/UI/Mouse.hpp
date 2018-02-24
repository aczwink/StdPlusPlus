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

namespace StdPlusPlus
{
    namespace UI
    {
        //Enums
        enum class MouseButton
        {
            Left,
            Right
        };

        //Functions
		STDPLUSPLUS_API bool GetMouseButtonState(MouseButton button);
        /**
         * Queries the current mouse pointer position in screen coordinates.
         *
         * @param x
         * @param y - Recap the Std++ coordinate system
         */
        STDPLUSPLUS_API void GetMousePointerPos(uint16 &x, uint16 &y);
    }
}
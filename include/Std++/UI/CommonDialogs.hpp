/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Strings/OldString.hpp>
#include "../Tuple.hpp"
#include "../Filesystem/Path.hpp"
#include "Window.hpp"

namespace StdXX
{
    namespace UI
    {
        namespace CommonDialogs
        {
            //Functions
            STDPLUSPLUS_API Path OpenFile(const Window &refParentWnd, const OldString &refTitle, const LinkedList<Tuple<OldString, OldString>> &refFilters, const Path &directory = Path());
            STDPLUSPLUS_API Path SaveFile(const Window *pParentWnd, const OldString &refTitle, const LinkedList<Tuple<OldString, OldString>> &refFilters);
        }
    }
}
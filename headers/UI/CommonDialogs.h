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
#include "../CTuple.h"
#include "../Filesystem/CPath.h"
#include "CWindow.h"

namespace ACStdLib
{
    namespace UI
    {
        namespace CommonDialogs
        {
            //Functions
            ACSTDLIB_API CPath OpenFile(const CWindow &refParentWnd, const CString &refTitle, const CLinkedList<CTuple<CString, CString>> &refFilters);
            ACSTDLIB_API CPath SaveFile(const CWindow *pParentWnd, const CString &refTitle, const CLinkedList<CTuple<CString, CString>> &refFilters);
            ACSTDLIB_API bool SelectExistingDirectory(const CString &refTitle, const CWindow &refWindow, const CFunction<bool(const CPath &)> &refCallback, CPath &refSelectedPath);
        }
    }
}
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
//Main header
#include "../headers/Debug.h"
//Local
#include "../headers/Containers/Strings/CString.h"

#ifdef _DEBUG
void ACStdLib::AssertionFailed(const char *pContext, const char *pMessage, const char *pFileName, uint32 lineNumber, const char *pFunctionName)
{
    AssertionFailed(pContext, CString(pMessage), pFileName, lineNumber, pFunctionName);
}
#endif
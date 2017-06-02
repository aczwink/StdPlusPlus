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
#include <ACStdLib/Debug.h>
//Local
#include <ACStdLib/Containers/Strings/String.hpp>

#ifdef _DEBUG
void ACStdLib::AssertionFailed(const char *pContext, const char *pMessage, const char *pFileName, uint32 lineNumber, const char *pFunctionName)
{
    AssertionFailed(pContext, String(pMessage), pFileName, lineNumber, pFunctionName);
}

void ACStdLib::AssertionFailed(float64 expect, float64 got, float64 epsilon, const char *fileName, uint32 lineNumber, const char *functionName)
{
	String message;

	message = "Expected: " + ToString(expect) + "\n";
	message += "Got: " + ToString(got) + "\n";
	message += "Epsilon: " + ToString(epsilon) + "\n";

	AssertionFailed("Float::AlmostEqual(expect, got, epsilon)", message, fileName, lineNumber, functionName);
}
#endif
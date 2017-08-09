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
#include "Definitions.h"

#ifdef _DEBUG
#define ASSERT(expression) if((expression) == 0){ACStdLib::AssertionFailed(#expression, "", __FILE__, __LINE__, __FUNCTION__);}
#define ASSERT_MSG(expression, message) if((expression) == 0){ACStdLib::AssertionFailed(#expression, message, __FILE__, __LINE__, __FUNCTION__);}
//extended asserts
//TODO: #13 - for the following line, alter Float<float64> to Float
#define ASSERT_FLOATS_EQUAL(expect, got, epsilon) if(Float<float64>::AlmostEqual(expect, got, epsilon) == false){ACStdLib::AssertionFailed(expect, got, epsilon, __FILE__, __LINE__, __FUNCTION__);}
#define ERROR(msg) ASSERT_MSG(false, msg)
#define NOT_IMPLEMENTED_ERROR ERROR("You've reached a point in the program that is not implemented.")

namespace ACStdLib
{
	//Forward declarations
	class String;

	void ACSTDLIB_API AssertionFailed(const char *pContext, const char *pMessage, const char *pFileName, uint32 lineNumber, const char *pFunctionName);
	void ACSTDLIB_API AssertionFailed(const char *pContext, const String &refMessage, const char *pFileName, uint32 lineNumber, const char *pFunctionName);
	void ACSTDLIB_API AssertionFailed(float64 expect, float64 got, float64 epsilon, const char *fileName, uint32 lineNumber, const char *functionName);
}
#else
#define ASSERT(expression)
#define ASSERT_MSG(expression, message)
#define NOT_IMPLEMENTED_ERROR
#endif
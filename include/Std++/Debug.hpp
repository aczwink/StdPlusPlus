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
#include "Definitions.h"

#ifdef _DEBUG
#define ASSERT(expression, message) {if((expression) == 0){StdPlusPlus::AssertionFailed(#expression, message, __FILE__, __LINE__, __FUNCTION__);}}
//extended asserts
#define ASSERT_FLOATS_EQUAL_64(expect, got, epsilon) if(Float<float64>::AlmostEqual(expect, got, epsilon) == false){StdPlusPlus::AssertionFailed(expect, got, epsilon, __FILE__, __LINE__, __FUNCTION__);}
//Errors
#define NOT_IMPLEMENTED_ERROR StdPlusPlus::RaiseNotImplementedError(__FILE__, __LINE__, __FUNCTION__)

namespace StdPlusPlus
{
	//Move declarations
	class String;

	void STDPLUSPLUS_API AssertionFailed(const char *pContext, const char *pMessage, const char *pFileName, uint32 lineNumber, const char *pFunctionName);
	void STDPLUSPLUS_API AssertionFailed(const char *pContext, const String &refMessage, const char *pFileName, uint32 lineNumber, const char *pFunctionName);
	void STDPLUSPLUS_API AssertionFailed(float64 expect, float64 got, float64 epsilon, const char *fileName, uint32 lineNumber, const char *functionName);
	void STDPLUSPLUS_API RaiseNotImplementedError(const char *fileName, uint32 lineNumber, const char *functionName);
}
#else
#define ASSERT(expression, message) {}
//extended asserts
#define ASSERT_FLOATS_EQUAL_64(expect, got, epsilon)
#define NOT_IMPLEMENTED_ERROR
#endif
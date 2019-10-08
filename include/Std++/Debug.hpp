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
#include "Definitions.h"

#ifdef XPC_BUILDTYPE_DEBUG
#define ASSERT(expression, message) {if((expression) == 0){StdXX::AssertionFailed(#expression, message, __FILE__, __LINE__, __FUNCTION__);}}
//extended asserts
#define ASSERT_FLOATS_EQUAL_64(expect, got, epsilon) if(Float<float64>::AlmostEqual(expect, got, epsilon) == false){StdXX::AssertionFailed(expect, got, epsilon, __FILE__, __LINE__, __FUNCTION__);}

namespace StdXX
{
	//Forward declarations
	class String;

	void AssertionFailed(const char *pContext, const char *pMessage, const char *pFileName, uint32 lineNumber, const char *pFunctionName);
	void AssertionFailed(const char *pContext, const String &refMessage, const char *pFileName, uint32 lineNumber, const char *pFunctionName);
	void AssertionFailed(float64 expect, float64 got, float64 epsilon, const char *fileName, uint32 lineNumber, const char *functionName);
}
#else
#define ASSERT(expression, message) {}
//extended asserts
#define ASSERT_FLOATS_EQUAL_64(expect, got, epsilon)
#endif

#ifdef XPC_BUILDTYPE_RELEASE
#define NOT_IMPLEMENTED_ERROR #error "Can not build in release mode with NOT_IMPLEMENTED_ERROR-marks."
#define RAISE(exceptionType) throw exceptionType()
#define THROW(exceptionType, ...) throw exceptionType(__VA_ARGS__)
#else
#define RAISE(exceptionType) throw exceptionType(__FILE__, __LINE__, __FUNCTION__)
#define THROW(exceptionType, ...) throw exceptionType(__VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
//Errors
#define NOT_IMPLEMENTED_ERROR StdXX::RaiseNotImplementedError(__FILE__, __LINE__, __FUNCTION__)

namespace StdXX
{
	void RaiseNotImplementedError(const char *fileName, uint32 lineNumber, const char *functionName);
}
#endif
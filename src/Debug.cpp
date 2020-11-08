/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
//Main header
#include <Std++/Debug.hpp>
//Local
#include <Std++/Errorhandling/Errors/AssertionError.hpp>
#include <Std++/Errorhandling/Errors/NotImplementedError.hpp>
#include <Std++/Containers/Strings/String.hpp>
//Namespaces
using namespace StdXX;

#ifdef XPC_BUILDTYPE_DEBUG
//Local functions
static void AssertEqualsFailed(const String& expected, const String& got, const char *fileName, uint32 lineNumber, const char *functionName)
{
	String message = u8"Expected: '" + expected + u8"' but got: '" + got + u8"'.";
	throw ErrorHandling::AssertionError(u8"expected == got", message, fileName, lineNumber, functionName);
}

//Namespace functions
void StdXX::AssertEqualsFailed(int64 expected, int64 got, const char *fileName, uint32 lineNumber, const char *functionName)
{
	::AssertEqualsFailed(String::Number(expected), String::Number(got), fileName, lineNumber, functionName);
}

void StdXX::AssertEqualsFailed(uint64 expected, uint64 got, const char *fileName, uint32 lineNumber, const char *functionName)
{
	::AssertEqualsFailed(String::Number(expected), String::Number(got), fileName, lineNumber, functionName);
}

void StdXX::AssertionFailed(const char *pContext, const char *pMessage, const char *pFileName, uint32 lineNumber, const char *pFunctionName)
{
	throw ErrorHandling::AssertionError(pContext, pMessage, pFileName, lineNumber, pFunctionName);
}

void StdXX::AssertionFailed(const char *pContext, const String &refMessage, const char *pFileName, uint32 lineNumber, const char *pFunctionName)
{
	throw ErrorHandling::AssertionError(pContext, refMessage, pFileName, lineNumber, pFunctionName);
}

void StdXX::AssertFloatsEqualFailed(float64 expect, float64 got, float64 epsilon, const char *fileName, uint32 lineNumber, const char *functionName)
{
	String message;

	message = u8"Expected: " + String::Number(expect) + u8"\n";
	message += u8"Got: " + String::Number(got) + u8"\n";
	message += u8"Epsilon: " + String::Number(epsilon) + u8"\n";

	throw ErrorHandling::AssertionError(u8"Float::AlmostEqual(expect, got, epsilon)", message, fileName, lineNumber, functionName);
}
#endif

#ifndef XPC_BUILDTYPE_RELEASE
void StdXX::RaiseNotImplementedError(const char *fileName, uint32 lineNumber, const char *functionName)
{
	throw ErrorHandling::NotImplementedError(fileName, lineNumber, functionName);
}
#endif
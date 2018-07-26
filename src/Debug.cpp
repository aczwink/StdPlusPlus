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
//Main header
#include <Std++/Debug.hpp>
//Local
#include <Std++/Errorhandling/AssertionError.hpp>
#include <Std++/Errorhandling/NotImplementedError.hpp>
#include <Std++/Containers/Strings/String.hpp>

#ifdef _DEBUG
void StdXX::AssertionFailed(const char *pContext, const char *pMessage, const char *pFileName, uint32 lineNumber, const char *pFunctionName)
{
	throw AssertionError(pContext, pMessage, pFileName, lineNumber, pFunctionName);
}

void StdXX::AssertionFailed(const char *pContext, const String &refMessage, const char *pFileName, uint32 lineNumber, const char *pFunctionName)
{
	throw AssertionError(pContext, refMessage, pFileName, lineNumber, pFunctionName);
}

void StdXX::AssertionFailed(float64 expect, float64 got, float64 epsilon, const char *fileName, uint32 lineNumber, const char *functionName)
{
	String message;

	message = u8"Expected: " + String::Number(expect) + u8"\n";
	message += u8"Got: " + String::Number(got) + u8"\n";
	message += u8"Epsilon: " + String::Number(epsilon) + u8"\n";

	throw AssertionError(u8"Float::AlmostEqual(expect, got, epsilon)", message, fileName, lineNumber, functionName);
}

void StdXX::RaiseNotImplementedError(const char *fileName, uint32 lineNumber, const char *functionName)
{
	throw NotImplementedError(fileName, lineNumber, functionName);
}
#endif
/*
 * Copyright (c) 2017-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Type/Global.hpp>
#include <Std++/Type.hpp>
#include <Std++/Type/Remove_cv.hpp>
#include "Definitions.h"

#ifdef XPC_BUILDTYPE_DEBUG
#define ASSERT(expression, message) {if((expression) == 0){StdXX::AssertionFailed(#expression, message, __FILE__, __LINE__, __FUNCTION__);}}
#define ASSERT_TRUE(expression) ASSERT(expression, u8"Value was expected to be true but is not.")
//extended asserts
#define ASSERT_EQUALS(expect, got) { auto a = (expect); auto b = (got); if( !( a == b ) ){ StdXX::AssertEqualsFailed(a, b, __FILE__, __LINE__, __FUNCTION__); } }
#define ASSERT_FLOATS_EQUAL_64(expect, got, epsilon) if(Float<float64>::AlmostEqual(expect, got, epsilon) == false){StdXX::AssertionFailed(expect, got, epsilon, __FILE__, __LINE__, __FUNCTION__);}

namespace _stdxx_
{
	template <typename T, typename U>
	struct CommonInteger8ByteExtended_WithoutSimplification : public StdXX::Type::Conditional<
	        StdXX::Type::IsSigned<T>::value,
	        typename StdXX::Type::CommonType<int64, typename StdXX::Type::CommonType<T, U>::type>::type,
	        typename StdXX::Type::CommonType<uint64, typename StdXX::Type::CommonType<T, U>::type>::type
	>{};

	template <typename T, typename U>
	struct CommonInteger8ByteExtended : public CommonInteger8ByteExtended_WithoutSimplification<
			typename StdXX::Type::RemoveConst<typename StdXX::Type::RemoveReference<T>::type>::type,
			typename StdXX::Type::RemoveConst<typename StdXX::Type::RemoveReference<U>::type>::type
	>{};
}

namespace StdXX
{
	//Forward declarations
	class String;

	void AssertEqualsFailed(int64 expected, int64 got, const char *fileName, uint32 lineNumber, const char *functionName);
	void AssertEqualsFailed(uint64 expected, uint64 got, const char *fileName, uint32 lineNumber, const char *functionName);
	void AssertionFailed(const char* context, const char8_t* message, const char *pFileName, uint32 lineNumber, const char *pFunctionName);
	void AssertionFailed(const char* context, const String &refMessage, const char *pFileName, uint32 lineNumber, const char *pFunctionName);
	void AssertFloatsEqualFailed(float64 expect, float64 got, float64 epsilon, const char *fileName, uint32 lineNumber, const char *functionName);

	template <typename T, typename U>
	inline typename Type::EnableIf<StdXX::Type::IsIntegral<T>::value>::type AssertEqualsFailed(const T& expected, const U& got, const char *fileName, uint32 lineNumber, const char *functionName)
	{
		typedef typename _stdxx_::CommonInteger8ByteExtended<T, U>::type targetType;
		AssertEqualsFailed(targetType(expected), targetType(got), fileName, lineNumber, functionName);
	}

	template <typename T, typename U>
	inline typename Type::EnableIf<!StdXX::Type::IsIntegral<T>::value>::type AssertEqualsFailed(const T& expected, const U& got, const char *fileName, uint32 lineNumber, const char *functionName)
	{
		AssertionFailed("expected == got", u8"Expected and received value differ.", fileName, lineNumber, functionName);
	}
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
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

//Definitions
#ifdef XPC_OS_WINDOWS
    #ifdef _STDPLUSPLUS_EXPORT
        #define STDPLUSPLUS_API __declspec(dllexport)
    #else
        #define STDPLUSPLUS_API __declspec(dllimport)
    #endif
#else
    #define STDPLUSPLUS_API
#endif

//Binary
#define LOW8(value) (value & 0xFF)
#define HIGH8(value) ((uint8)(value >> 8))

#define LOW16(value) (value & 0xFFFF)
#define HIGH16(value) ((uint16)(value >> 16))
#define MAKE16(lo8, hi8) ( (lo8 & 0xFF) | ((hi8 & 0xFF) << 8) )

#define LOW32(value) ((uint32)value & UINT32_MAX)
#define HIGH32(value) ((uint32)(value >> 32))
#define MAKE32(lo16, hi16) ( (lo16 & 0xFFFF) | ((hi16 & 0xFFFF) << 16) )

#define MAKE64(lo32, hi32) ( (uint64)((lo32) & Unsigned<uint32>::Max()) | ((uint64)((hi32) & Unsigned<uint32>::Max()) << 32) )

#define XOR ^

//Binary prefixes
#define KiB (1024)
#define MiB (1024 * KiB)

//Limits
#define UINT16_MAX ((uint32)0xFFFF)
#define UNICODE_MAX ((uint32)0x10FFFF)

//Math
#define IS_POWER_OF_TWO(x) (((x) != 0) && (((x) & ((x) - 1)) == 0))

//Types
#ifdef _MSC_VER
typedef unsigned __int8 byte;
typedef __int8 int8;
typedef unsigned __int8 uint8;
typedef __int16 int16;
typedef unsigned __int16 uint16;
typedef __int32 int32;
typedef unsigned __int32 uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
typedef unsigned char byte;
typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed long long int64;
typedef unsigned long long uint64;
#endif
typedef float float32;
typedef double float64;

#define IS_INSTANCE_OF(ptr, classType) (dynamic_cast<const classType *>(ptr))

//Things from C/C++ Standard Headers
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
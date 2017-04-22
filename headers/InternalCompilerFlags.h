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

//determine cpu architecture
#if defined(_M_X64) or defined(__x86_64__)
#define _AC_ARCH_X64
#else
#error "undefined architecture"
#endif

//endianness
#ifdef _AC_ARCH_X64
#define _AC_ENDIAN_LITTLE
#else
#error "undefined endianness"
#endif

//operating system
#ifdef _WIN32
#define _AC_OS_WINDOWS
#elif __linux__
#define _AC_OS_LINUX
#else
#error "undefined operating system"
#endif

//compiler
#ifdef __GNUC__
#define _AC_COMPILER_GCC
#endif
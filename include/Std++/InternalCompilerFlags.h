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

//determine cpu architecture
#if defined(_M_X64) || defined(__x86_64__)
#define _STDPP_ARCH_X64
#else
#error "undefined architecture"
#endif

//endianness
#ifdef _STDPP_ARCH_X64
#define _STDPP_ENDIAN_LITTLE
#else
#error "undefined endianness"
#endif

//operating system
#ifdef _WIN32
#define _AC_OS_WINDOWS
#elif __linux__
#define _STDPP_OS_LINUX
#else
#error "undefined operating system"
#endif

//compiler
#ifdef __GNUC__
#define _STDPP_COMPILER_GCC
#endif
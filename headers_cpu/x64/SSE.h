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
//All x64 processors support SSE and SSE2
#include <xmmintrin.h> //SSE
#include <emmintrin.h> //SSE2
//Steam hardware survey: http://store.steampowered.com/hwsurvey
#include <pmmintrin.h> //SSE3 according to steam supported by 99.99%
#include <smmintrin.h> //SSE4.1 according to steam supported by 87.68%
//TODO: could be that SSE4.1 needs to be thrown out if compiled for older processor

#ifdef __GNUC__
#define _MM_ALIGN16 __attribute__((aligned (16)))
#endif
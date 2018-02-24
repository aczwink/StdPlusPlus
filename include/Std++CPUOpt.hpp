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
#include <Std++/InternalCompilerFlags.h>

//now include the correct architecture header
#ifdef _AC_ARCH_X64
//vector types
#include <Std++CPUOpt/x64/vec2f64.h>
#include <Std++CPUOpt/x64/vec4f32.h>
#include <Std++CPUOpt/x64/vec4i32.h>
#include <Std++CPUOpt/x64/vec4u32.h>
#endif
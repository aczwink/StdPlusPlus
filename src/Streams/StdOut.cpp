/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of Std++.
 *
 * Std++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or#include "../../headers/Streams/CStdOut.h"
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
//Class Header
#include <Std++/Streams/Writers/StdOut.hpp>
//Global
#include <stdio.h>
//Namespaces
using namespace StdXX;

//Global Variables
StdErr STDPLUSPLUS_API StdXX::stdErr;
StdOut STDPLUSPLUS_API StdXX::stdOut;

//StdErr Public methods
void StdErr::Flush()
{
	fflush(stderr);
}

uint32 StdErr::WriteBytes(const void *pSource, uint32 count)
{
    return (uint32)fwrite(pSource, 1, count, stderr);
}

//StdOut Public methods
void StdOut::Flush()
{
	fflush(stdout);
}

uint32 StdOut::WriteBytes(const void *pSource, uint32 count)
{
    return (uint32)fwrite(pSource, 1, count, stdout);
}
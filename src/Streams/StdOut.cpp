/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/StdOut.hpp>
//Global
#include <stdio.h>
//Namespaces
using namespace StdXX;

//Global Variables
CStdErr STDPLUSPLUS_API StdXX::stdErr;
StdOut STDPLUSPLUS_API StdXX::stdOut;

//CStdErr Public methods
void CStdErr::WriteByte(byte b)
{
    putc(b, stderr);
}

uint32 CStdErr::WriteBytes(const void *pSource, uint32 count)
{
    return (uint32)fwrite(pSource, 1, count, stderr);
}

//StdOut Public methods
void StdOut::WriteByte(byte b)
{
    putc(b, stdout);
}

uint32 StdOut::WriteBytes(const void *pSource, uint32 count)
{
    return (uint32)fwrite(pSource, 1, count, stdout);
}
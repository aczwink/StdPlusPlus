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
//Local
#include "main.h"
#include "../headers/__InitAndShutdown.h"

//Prototypes
void StartUserMemoryLogging();

//Global functions
int32 _ACMain(const CString &refProgramName, const CLinkedList<CString> &refArgs)
{
    int32 exitCode;

    exitCode = -1;

    InitACStdLib();
    StartUserMemoryLogging();
    try
    {
        exitCode = Main(refProgramName, refArgs);
    }
    catch(const ErrorHandling::AException &refE)
    {
        ASSERT_MSG(false, "Uncaught exception: " + refE.GetDescription());
    }
    catch(...)
    {
        ASSERT_MSG(false, "Uncaught exception (not ACStdLib)");
    }
    ShutdownACStdLib();

    return exitCode;
}
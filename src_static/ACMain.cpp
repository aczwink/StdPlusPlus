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
#include "../include/ACStdLib/__InitAndShutdown.h"
#include "../include/ACStdLib/Containers/Strings/String.hpp"

//Prototypes
#ifdef _DEBUG
ACSTDLIB_API void StartUserMemoryLogging();
#endif

//Global functions
int32 _ACMain(const String &programName, const FixedArray<String> &args)
{
    int32 exitCode = -1;

    InitACStdLib();
#ifdef _DEBUG
    StartUserMemoryLogging();
#endif
    try
    {
        exitCode = Main(programName, args);
    }
    catch(const ErrorHandling::Exception &e)
    {
        ASSERT_MSG(false, u8"Uncaught exception: " + e.GetDescription());
    }
    catch(...)
    {
        ASSERT_MSG(false, "Uncaught exception (not ACStdLib)");
    }
    ShutdownACStdLib();

    return exitCode;
}
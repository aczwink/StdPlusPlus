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
//Local
#include "main.hpp"
#include <Std++.hpp>
#ifdef _STDPP_OS_WINDOWS
#include <Windows.h>
#endif

//Prototypes
#ifdef _DEBUG
STDPLUSPLUS_API void StartUserMemoryLogging();
#endif

//Local functions
static void ReportError(const String &message1, const String &message2)
{
	stdErr << message1 << message2 << endl;
#ifdef _STDPP_OS_WINDOWS
	MessageBoxW(NULL, (LPCWSTR)message2.ToUTF16().GetRawZeroTerminatedData(), (LPCWSTR)message1.ToUTF16().GetRawZeroTerminatedData(), MB_ICONERROR | MB_TASKMODAL);
#endif
#ifdef _STDPP_COMPILER_MSVC
	OutputDebugStringW((LPCWSTR)(message1 + message2).ToUTF16().GetRawZeroTerminatedData());
#endif
}

//Global functions
int32 _StdPlusPlusMain(const String &programName, const FixedArray<String> &args)
{
    int32 exitCode = -1;

    InitStdPlusPlus();
#ifdef _DEBUG
    StartUserMemoryLogging();
#endif
    try
    {
        exitCode = Main(programName, args);
    }
    catch(const Exception &e)
    {
		ReportError(u8"Uncaught exception: ", e.GetDescription());
    }
	catch(const Error &e)
	{
		ReportError(u8"ERROR: ", e.GetDescription());
	}
    catch(...)
    {
		ReportError(u8"Uncaught exception (not StdPlusPlus)", String());
    }
	ShutdownStdPlusPlus();

    return exitCode;
}
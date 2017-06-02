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
//corresponding header
#include <ACStdLib/Debug.h>
//Global
#include <iostream>
//Local
#include <ACStdLib/Containers/Strings/String.hpp>
#include <ACStdLib/Containers/Strings/StringUtil.h>
#include <ACStdLib/Streams/CStdOut.h>
//Namespaces
using namespace ACStdLib;

#ifdef _DEBUG
//Prototypes
void ShutdownACStdLib();

void ACStdLib::AssertionFailed(const char *pContext, const String &refMessage, const char *pFileName, uint32 lineNumber, const char *pFunctionName)
{
    String message;

    message = "Expression: ";
    message += pContext;
    message += "\n";
    message += refMessage;
    message += "\nFile: ";
    message += pFileName;
    message += " (" + ToString((uint64)lineNumber) + ")\nFunction: ";
    message += pFunctionName;

    stdErr << message << endl;

	static bool shutdown = true;
	if(shutdown)
	{
		shutdown = false;
		ShutdownACStdLib(); //call the shutdown function that will report memory leaks and so on
	}
    exit(1);
}
#endif
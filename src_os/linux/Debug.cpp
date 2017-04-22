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
#include <ACStdLib/Containers/Strings/CString.h>
#include <ACStdLib/Containers/Strings/String.h>
#include <ACStdLib/Streams/CStdOut.h>
//Namespaces
using namespace ACStdLib;

#ifdef _DEBUG
void ACStdLib::AssertionFailed(const char *pContext, const CString &refMessage, const char *pFileName, uint32 lineNumber, const char *pFunctionName)
{
    CString message;

    message = "Expression: ";
    message += pContext;
    message += "\n";
    message += refMessage;
    message += "\nFile: ";
    message += pFileName;
    message += " (" + ToString((uint64)lineNumber) + ")\nFunction: ";
    message += pFunctionName;

    stdErr << message << endl;
    exit(1);
}
#endif
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
//corresponding header
#include <Std++/Debug.hpp>
//Global
#include <Windows.h>
//Local
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Containers/Strings/StringUtil.h>

#ifdef _DEBUG
void StdPlusPlus::AssertionFailed(const char *pContext, const String &refMessage, const char *pFileName, uint32 lineNumber, const char *pFunctionName)
{
	String message;

	message = "Expression: ";
	message += pContext;
	message += "\n";
	message += refMessage;
	message += "\nFile: ";
	message += pFileName;
	message += " (" + String::Number(lineNumber) + ")\nFunction: ";
	message += pFunctionName;

	MessageBoxW(NULL, (LPCWSTR)message.ToUTF16().GetRawZeroTerminatedData(), L"Assertion", MB_ICONERROR | MB_TASKMODAL);
	abort();
}
#endif
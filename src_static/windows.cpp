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
#ifdef _AC_OS_WINDOWS
#include <Windows.h>
#include "main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
	int32 nArgs;
	LPWSTR *pArgList;
	CString programName;
	CLinkedList<CString> args;

	pArgList = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	programName = CUTF16String((uint16 *)pArgList[0]);
	for(int32 i = 1; i < nArgs; i++)
	{
		args.InsertTail(CUTF16String((uint16 *)pArgList[i]));
	}
	LocalFree(pArgList);

	return ACMain(programName, args);
}
#endif
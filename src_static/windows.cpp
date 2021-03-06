/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
#ifdef XPC_OS_WINDOWS
#include <Windows.h>
#include "main.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
	int32 nArgs;
	LPWSTR* pArgList = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	String programName = String::CopyRawString((uint16 *)pArgList[0]);

	FixedArray<String> args(nArgs-1);
	for(int32 i = 1; i < nArgs; i++)
	{
		args[i-1] = String::CopyRawString((uint16 *)pArgList[i]);
	}
	LocalFree(pArgList);

	return _StdPlusPlusMain(programName, args);
}
#endif
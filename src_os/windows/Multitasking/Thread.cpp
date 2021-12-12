/*
* Copyright (c) 2017-2018,2021 Amir Czwink (amir130@hotmail.de)
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
//Class header
#include <Std++/Multitasking/Thread.hpp>
//Global
#include <Windows.h>
//Local
#include <Std++/Function.hpp>
//Namespaces
using namespace StdXX;

//Local functions
static DWORD WINAPI RunThreadByClassMethod(LPVOID pFunction)
{
	Function<int32()> *pFunctionTyped = (Function<int32()> *)pFunction;
	int32 exitCode = (*pFunctionTyped)();
	delete pFunctionTyped;

	return exitCode;
}

//Destructor
Thread::~Thread()
{
	CloseHandle(this->systemHandle);
}

//Public methods
void Thread::Join(uint64 duration)
{
	DWORD ms = duration / 1000 / 1000;
	if ((ms == 0) && (duration != 0))
		ms = 1;
	WaitForSingleObject(this->systemHandle, ms);
}

void Thread::Start()
{
	ASSERT(this->systemHandle == nullptr, u8"Can't start an already started thread");
	
	DWORD threadId;
	
	Function<int32()> *pFunction = new Function<int32()>(&Thread::ThreadMain, this);
	this->systemHandle = CreateThread(nullptr, 0, RunThreadByClassMethod, pFunction, 0, &threadId);
}
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
//Class header
#include <Std++/Multitasking/Thread.hpp>
//Global
#include <Windows.h>
//Local
#include <Std++/Function.hpp>
//Namespaces
using namespace StdXX;

//Local functions
static DWORD WINAPI RunThreadByFuncPointer(LPVOID pFuncPtr)
{
	return ((ThreadFunction)pFuncPtr)();
}

static DWORD WINAPI RunThreadByFunctor(LPVOID pFunction)
{
	Function<int32()> *pFunctionTyped = (Function<int32()> *)pFunction;
	int32 exitCode = (*pFunctionTyped)();
	
	return exitCode;
}

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
bool Thread::Join(uint64 duration)
{
	DWORD ms = duration / 1000 / 1000;
	if ((ms == 0) && (duration != 0))
		ms = 1;
	return WaitForSingleObject(this->systemHandle, ms) == WAIT_TIMEOUT;
}

void Thread::Start()
{
	ASSERT(this->systemHandle == nullptr, "Can't start an already started thread");
	
	DWORD threadId;
	if (this->function)
		this->systemHandle = CreateThread(nullptr, 0, RunThreadByFuncPointer, this->function, 0, &threadId);
	else if (this->functor.IsBound())
	{
		this->systemHandle = CreateThread(nullptr, 0, RunThreadByFunctor, (void *)&this->functor, 0, &threadId);
	}
	else
	{
		Function<int32()> *pFunction = new Function<int32()>(&Thread::ThreadMain, this);
		this->systemHandle = CreateThread(nullptr, 0, RunThreadByClassMethod, pFunction, 0, &threadId);
	}
}
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
#include <Std++/Multitasking/ConditionVariable.hpp>
//Global
#include <Windows.h>
//Local
#include <Std++/Memory.h>
#include <Std++/Multitasking/Mutex.hpp>
//Namespaces
using namespace StdPlusPlus;
//Definitions
#define THIS ((CONDITION_VARIABLE *)this->systemHandle)

//Constructor
ConditionVariable::ConditionVariable()
{
	this->systemHandle = MemAlloc(sizeof(CONDITION_VARIABLE));
	InitializeConditionVariable(THIS);
}

//Destructor
ConditionVariable::~ConditionVariable()
{
	MemFree(this->systemHandle);
}

//Public methods
void ConditionVariable::Broadcast()
{
	WakeAllConditionVariable(THIS);
}

void ConditionVariable::Signal()
{
	WakeConditionVariable(THIS);
}

void ConditionVariable::Wait(Mutex &mutex)
{
	SleepConditionVariableCS(THIS, (CRITICAL_SECTION *)mutex.systemHandle, INFINITE);
}
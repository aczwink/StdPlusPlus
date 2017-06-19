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
//Class header
#include <ACStdLib/Multitasking/ConditionVariable.hpp>
//Global
#include <pthread.h>
//Local
#include <ACStdLib/Memory.h>
#include <ACStdLib/Multitasking/Mutex.hpp>
//Namespaces
using namespace ACStdLib;
//Definitions
#define THIS ((pthread_cond_t *)this->systemHandle)

//Constructor
ConditionVariable::ConditionVariable()
{
	this->systemHandle = MemAlloc(sizeof(pthread_cond_t));
	pthread_cond_init(THIS, nullptr);
}

//Destructor
ConditionVariable::~ConditionVariable()
{
	pthread_cond_destroy(THIS);
	MemFree(this->systemHandle);
}

//Public methods
void ConditionVariable::Signal()
{
	pthread_cond_signal(THIS);
}

void ConditionVariable::Wait(Mutex &mutex)
{
	pthread_cond_wait(THIS, (pthread_mutex_t *)mutex.systemHandle);
}
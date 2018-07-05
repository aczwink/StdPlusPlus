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
#include <Std++/Multitasking/Mutex.hpp>
//Global
#include <pthread.h>
//Local
#include <Std++/Memory.h>
//Namespaces
using namespace StdPlusPlus;
//Definitions
#define THIS ((pthread_mutex_t *)this->systemHandle)

//Constructor
Mutex::Mutex()
{
	this->systemHandle = MemAlloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(THIS, nullptr);
}

//Destructor
Mutex::~Mutex()
{
	pthread_mutex_destroy(THIS);
	MemFree(this->systemHandle);
}

//Public methods
void Mutex::Lock()
{
	pthread_mutex_lock(THIS);
}

void Mutex::Unlock()
{
	pthread_mutex_unlock(THIS);
}
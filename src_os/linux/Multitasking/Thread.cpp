/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include <pthread.h>
//Namespaces
using namespace StdXX;

//Public methods
void Thread::Join(uint64 duration)
{
    if(this->IsAlive())
    {
        timespec ts;
        ts.tv_sec = duration / 1000000000;
        ts.tv_nsec = duration % 1000000000;

        pthread_timedjoin_np((pthread_t)this->systemHandle, nullptr, &ts);
    }
}
/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/Mathematics.hpp>
#include <Std++/Time/Clock.hpp>
#include <Std++/Time/TimeMisc.hpp>
//Namespaces
using namespace StdXX;

//Public methods
void Thread::Join(uint64 duration)
{
	Clock c;

	c.Start();
	while(this->IsAlive() && (duration != 0))
	{
		uint64 sleepDur = Math::Min(uint64(1000), duration); //sleep for max one micro sec
		Sleep(sleepDur);

		uint64 timeSlept = c.GetElapsedNanoseconds();
		if(timeSlept > duration)
			duration = 0;
		else
			duration -= timeSlept;
		c.Start();
	}
	//pthread_join((pthread_t) this->systemHandle, nullptr);
}
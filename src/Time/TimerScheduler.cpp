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
#include <ACStdLib/Time/TimerScheduler.hpp>
//Local
#include <ACStdLib/Time/Clock.hpp>
#include <ACStdLib/Time/TimeMisc.hpp>
#include <ACStdLib/Natural.hpp>
//Namespaces
using namespace ACStdLib;

//Constructor
TimerScheduler::TimerScheduler() : oneShotSchedulerThread(Function<int32()>(&TimerScheduler::OneShotSchedulerThreadMain, this))
{
	this->shutdown = false;

	this->oneShotSchedulerThread.Start();
}

//Destructor
TimerScheduler::~TimerScheduler()
{
	this->shutdown = true;

	this->oneShot.workLock.Lock();
	this->oneShot.workSignal.Signal();
	this->oneShot.workLock.Unlock();

	this->oneShotSchedulerThread.Join();
}

//Eventhandlers
int32 TimerScheduler::OneShotSchedulerThreadMain()
{
	Clock c;

	while(true)
	{
		if(this->shutdown)
			break;

		//wait for work
		this->oneShot.workLock.Lock();
		this->oneShot.workSignal.Wait(this->oneShot.workLock);
		this->oneShot.workLock.Unlock();

		//handle timers
		c.Start();
		this->oneShot.timerQueueLock.Lock();
		while(!this->oneShot.timerQueue.IsEmpty())
		{
			if(this->shutdown)
			{
				this->oneShot.timerQueueLock.Unlock();
				break;
			}

			uint64 minWaitTime_usec = Natural<uint64>::Max();
			for(auto it = this->oneShot.timerQueue.begin(); it != this->oneShot.timerQueue.end(); )
			{
				(*it).leftTime_usec -= c.GetElapsedMicroseconds();
				if((*it).leftTime_usec <= 0)
				{
					this->oneShot.timerQueueLock.Unlock(); //we need to free the lock here because the timer handler might install a new one-shot timer
					(*it).timer->timedOutCallback();
					this->oneShot.timerQueueLock.Lock();
					it.Remove();
				}
				else
				{
					minWaitTime_usec = MIN(minWaitTime_usec, (uint64)(*it).leftTime_usec);
					++it;
				}
			}

			c.Start(); //restart the clock
			if(!this->oneShot.timerQueue.IsEmpty())
			{
				this->oneShot.timerQueueLock.Unlock();
				Sleep(minWaitTime_usec * 1000);
				this->oneShot.timerQueueLock.Lock();
			}
		}
		this->oneShot.timerQueueLock.Unlock();
	}

	return EXIT_SUCCESS;
}


//Public methods
void TimerScheduler::AddOneShotTimer(uint32 timeOut_usec, Timer *timer)
{
	this->oneShot.timerQueueLock.Lock();
	this->oneShot.timerQueue.InsertTail({timeOut_usec, timer});
	this->oneShot.timerQueueLock.Unlock();

	this->oneShot.workLock.Lock();
	this->oneShot.workSignal.Signal();
	this->oneShot.workLock.Unlock();
}

void TimerScheduler::RemoveOneShotTimer(Timer *timer)
{
	AutoLock autoLock(this->oneShot.timerQueueLock);
	for(auto it = this->oneShot.timerQueue.begin(); it != this->oneShot.timerQueue.end(); ++it)
	{
		if ((*it).timer == timer)
		{
			this->oneShot.timerQueue.Remove(it.GetIndex());
			break;
		}
	}
}
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
//Local
#include "../Containers/LinkedList/LinkedList.hpp"
#include "../Multitasking/ConditionVariable.hpp"
#include "../Multitasking/Mutex.hpp"
#include "../Multitasking/Thread.hpp"
#include "../Time/Timer.hpp"

namespace ACStdLib
{
	class TimerScheduler
	{
		struct TimerEntry
		{
			int64 leftTime_usec;
			Timer *timer;
		};
	public:
		//Constructor
		TimerScheduler();

		//Destructor
		~TimerScheduler();

		//Methods
		void AddOneShotTimer(uint32 timeOut_usec, Timer *timer);
		void RemoveOneShotTimer(Timer *timer);

	private:
		//Members
		bool shutdown;
		Thread oneShotSchedulerThread;
		struct
		{
			Mutex workLock;
			ConditionVariable workSignal;
			LinkedList<TimerEntry> timerQueue;
			Mutex timerQueueLock;
		} oneShot;

		//Eventhandlers
		int32 OneShotSchedulerThreadMain();
	};
}
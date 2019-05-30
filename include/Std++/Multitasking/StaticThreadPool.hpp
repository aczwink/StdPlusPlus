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
#pragma once
//Local
#include <Std++/Containers/Array/FixedArray.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#include "../Function.hpp"
#include "../Containers/LinkedList/LinkedList.hpp"
#include "ConditionVariable.hpp"
#include "Multitasking.hpp"
#include "Mutex.hpp"
#include "Thread.hpp"

namespace StdXX
{
    class STDPLUSPLUS_API StaticThreadPool
    {
        class WorkerThread;
        friend class WorkerThread;
    public:
        //Constructor
		explicit StaticThreadPool(uint32 nThreads = GetHardwareConcurrency());

		StaticThreadPool(const StaticThreadPool&) = delete;

        //Destructor
        ~StaticThreadPool();

		//Operators
		StaticThreadPool& operator=(const StaticThreadPool&) = delete;

        //Methods
        void EnqueueTask(const Function<void()>& task);
		void EnqueueTask(Function<void()>&& task);
		void WaitForAllTasksToComplete();

        //Inline
        inline uint32 GetNumberOfActiveThreads()
        {
        	AutoLock lock(this->activeThreadsLock);

        	return this->nActiveThreads;
        }

        inline uint32 GetNumberOfThreads() const
        {
            return this->workers.GetNumberOfElements();
        }

    private:
        //Members
    	FixedArray<UniquePointer<WorkerThread>> workers;

        uint32 nActiveThreads;
        Mutex activeThreadsLock;
        ConditionVariable activeThreadsSignal;

        LinkedList<Function<void()>> taskQueue;
        Mutex taskQueueLock;
        ConditionVariable taskQueueSignal;
    };
}
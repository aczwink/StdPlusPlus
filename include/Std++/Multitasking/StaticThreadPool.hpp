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
#pragma once
//Local
#include "../Function.hpp"
#include "../Containers/LinkedList/LinkedList.hpp"
#include "ConditionVariable.hpp"
#include "Mutex.hpp"
#include "Thread.hpp"

namespace StdXX
{
    class STDPLUSPLUS_API StaticThreadPool
    {
        class CWorkerThread;
        friend class CWorkerThread;
    public:
        //Constructors
        StaticThreadPool();

        //Destructor
        ~StaticThreadPool();

        //Methods
        void EnqueueTask(const Function<void()> &refTask);

        //Inline
        inline uint32 GetNumberOfActiveThreads()
        {
            uint32 result;

            this->activeThreadsLock.Lock();
            result = this->nActiveThreads;
            this->activeThreadsLock.Unlock();

            return result;
        }

        inline uint32 GetNumberOfThreads() const
        {
            return this->nThreads;
        }

        inline void WaitForAllTasksToComplete()
        {
            //wait until all threads stop working
            this->activeThreadsLock.Lock();

            while(this->nActiveThreads > 0)
                this->activeThreadsSignal.Wait(this->activeThreadsLock);

            this->activeThreadsLock.Unlock();
        }

    private:
        //Members
        uint32 nThreads;
        CWorkerThread **ppThreads;

        uint32 nActiveThreads;
        Mutex activeThreadsLock;
        ConditionVariable activeThreadsSignal;

        LinkedList<Function<void()>> taskQueue;
        Mutex taskQueueLock;
        ConditionVariable taskQueueSignal;
    };
}
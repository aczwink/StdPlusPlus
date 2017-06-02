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
#pragma once
//Local
#include "ACStdLib/Function.hpp"
#include "../Containers/LinkedList/LinkedList.hpp"
#include "CConditionVariable.h"
#include "CMutex.h"
#include "Thread.hpp"

namespace ACStdLib
{
    class ACSTDLIB_API CStaticThreadPool
    {
        class CWorkerThread;
        friend class CWorkerThread;
    private:
        //Members
        uint32 nThreads;
        CWorkerThread **ppThreads;

        uint32 nActiveThreads;
        CMutex activeThreadsLock;
        CConditionVariable activeThreadsSignal;

        LinkedList<Function<void()>> taskQueue;
        CMutex taskQueueLock;
        CConditionVariable taskQueueSignal;

    public:
        //Constructors
        CStaticThreadPool();

        //Destructor
        ~CStaticThreadPool();

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
    };
}
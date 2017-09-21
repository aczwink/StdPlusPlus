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
#include <ACStdLib/Multitasking/StaticThreadPool.hpp>
//Local
#include <ACStdLib/Multitasking/Multitasking.hpp>
//Namespaces
using namespace ACStdLib;

//Worker Thread
class StaticThreadPool::CWorkerThread : public Thread
{
public:
	//Members
	volatile bool shutdown;

	//Constructor
	inline CWorkerThread(StaticThreadPool *pOwner)
	{
		this->pOwner = pOwner;
		this->shutdown = false;
	}

private:
	//Members
	StaticThreadPool *pOwner;

	//Event handlers
	int32 ThreadMain()
	{
		bool working;
		Function<void()> currentTask;

		working = false;

		while(!this->shutdown)
		{
			this->GetNextTask(currentTask, working);
			if(this->shutdown)
				break;

			this->SetWorking(working);

			//execute task
			currentTask();
		}

		this->SetNotWorking(working);

		return EXIT_SUCCESS;
	}

	//Methods
	void GetNextTask(Function<void()> &refTask, bool &refWorking)
	{
		this->pOwner->taskQueueLock.Lock();
		while(this->pOwner->taskQueue.IsEmpty())
		{
			this->SetNotWorking(refWorking);
			this->pOwner->taskQueueSignal.Wait(this->pOwner->taskQueueLock);

			//check if kill thread
			if(this->shutdown)
			{
				this->pOwner->taskQueueLock.Unlock();
				return;
			}
		}

		refTask = this->pOwner->taskQueue.PopFront();
		this->pOwner->taskQueueLock.Unlock();
	}

	//Inline
	inline void SetNotWorking(bool &refWorking)
	{
		if(refWorking)
		{
			this->pOwner->activeThreadsLock.Lock();
			this->pOwner->nActiveThreads--;
			this->pOwner->activeThreadsSignal.Signal();
			this->pOwner->activeThreadsLock.Unlock();

			refWorking = false;
		}
	}

	inline void SetWorking(bool &refWorking)
	{
		if(!refWorking)
		{
			this->pOwner->activeThreadsLock.Lock();
			this->pOwner->nActiveThreads++;
			this->pOwner->activeThreadsSignal.Signal();
			this->pOwner->activeThreadsLock.Unlock();

			refWorking = true;
		}
	}
};






//Constructors
StaticThreadPool::StaticThreadPool()
{
	this->nThreads = GetHardwareConcurrency();
	this->nActiveThreads = 0;

	this->ppThreads = (StaticThreadPool::CWorkerThread **)MemAlloc(this->nThreads * sizeof(*this->ppThreads));
	for(uint32 i = 0; i < this->nThreads; i++)
	{
		this->ppThreads[i] = new StaticThreadPool::CWorkerThread(this);
		this->ppThreads[i]->Start();
	}
}

//Destructor
StaticThreadPool::~StaticThreadPool()
{
	uint32 i;

	//ask all threads to terminate
	for(i = 0; i < this->nThreads; i++)
		this->ppThreads[i]->shutdown = true;

	this->taskQueueSignal.Broadcast();

	//wait for all threads to terminate
	for(i = 0; i < this->nThreads; i++)
	{
		this->ppThreads[i]->Join();
		delete this->ppThreads[i];
	}

	MemFree(this->ppThreads);
}

//Public methods
void StaticThreadPool::EnqueueTask(const Function<void()> &refTask)
{
	this->taskQueueLock.Lock();
	this->taskQueue.InsertTail(refTask);
	this->taskQueueSignal.Signal();
	this->taskQueueLock.Unlock();
}
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
//Class header
#include <Std++/Multitasking/StaticThreadPool.hpp>
//Namespaces
using namespace StdXX;

//Worker Thread
class StaticThreadPool::WorkerThread : public Thread
{
	friend class StaticThreadPool;
public:
	//Constructor
	WorkerThread(StaticThreadPool* owner) : shutdown(false), working(false), owner(owner)
	{
	}

private:
	//Members
	volatile bool shutdown;
	bool working;
	StaticThreadPool* owner;

	//Methods
	Function<void()> GetNextTask()
	{
		this->owner->taskQueueLock.Lock();
		while(this->owner->taskQueue.IsEmpty())
		{
			this->SetNotWorking();
			this->owner->taskQueueSignal.Wait(this->owner->taskQueueLock);

			//check if kill thread
			if(this->shutdown)
			{
				this->owner->taskQueueLock.Unlock();
				return [](){};
			}
		}

		this->SetWorking();

		Function<void()> task = this->owner->taskQueue.PopFront();
		//this->owner->taskQueueSignal.Signal();
		this->owner->taskQueueLock.Unlock();

		return task;
	}

	void Run() override
	{
		while(!this->shutdown)
		{
			Function<void()> currentTask = this->GetNextTask();
			if(this->shutdown)
				break;
			currentTask(); //execute task
		}

		this->SetNotWorking();
	}

	//Inline
	inline void SetNotWorking()
	{
		if(this->working)
		{
			this->owner->activeThreadsLock.Lock();
			this->owner->nActiveThreads--;
			this->working = false;
			this->owner->activeThreadsSignal.Signal();
			this->owner->activeThreadsLock.Unlock();
		}
	}

	inline void SetWorking()
	{
		if(!this->working)
		{
			this->owner->activeThreadsLock.Lock();
			this->owner->nActiveThreads++;
			this->working = true;
			this->owner->activeThreadsSignal.Signal();
			this->owner->activeThreadsLock.Unlock();
		}
	}
};

//Constructor
StaticThreadPool::StaticThreadPool(uint32 nThreads) : workers(nThreads), nActiveThreads(0)
{
	for(uint32 i = 0; i < nThreads; i++)
	{
		this->workers[i] = new WorkerThread(this);
		this->workers[i]->Start();
	}
}

//Destructor
StaticThreadPool::~StaticThreadPool()
{
	//ask all threads to terminate
	for(auto& w : this->workers)
		w->shutdown = true;

	this->taskQueueLock.Lock();
	this->taskQueueSignal.Broadcast();
	this->taskQueueLock.Unlock();

	//wait for all threads to terminate
	for(auto& w : this->workers)
	{
		while(w->IsAlive())
			w->Join();
	}
}

//Public methods
void StaticThreadPool::EnqueueTask(const Function<void()> &refTask)
{
	this->taskQueueLock.Lock();
	this->taskQueue.InsertTail(refTask);
	this->taskQueueSignal.Signal();
	this->taskQueueLock.Unlock();
}

void StaticThreadPool::WaitForAllTasksToComplete()
{
	bool running = true;
	while(running)
	{
		//wait until all threads stopped working
		this->activeThreadsLock.Lock();
		while(this->nActiveThreads > 0)
			this->activeThreadsSignal.Wait(this->activeThreadsLock);
		this->activeThreadsLock.Unlock();

		//now check if the task queue is empty
		this->taskQueueLock.Lock();
		running = !this->taskQueue.IsEmpty();
		/* important!: usually we would wait now on taskQueueSignal.
		 * However, when a task is gathered from the task queue by a worker it does a signal i.e. wake up SOME
		 * thread - not necessarily us. If its another worker he will check the queue and go to sleep in case there are
		 * no further tasks. This would require a broadcast then instead of a signal, which would be a waste of threads
		 * cycles. If the task queue is not empty,
		 * then threads will be working on it, thus we can wait on that signal instead (above).
		 * */
		this->taskQueueLock.Unlock();

		/*
		 * we have to double check this, since a thread can become active right after we checked for active threads.
		 * it may take the last task from the queue and we will think that all tasks are completed.
		 * */
		this->activeThreadsLock.Lock();
		while(this->nActiveThreads > 0)
		{
			this->activeThreadsSignal.Wait(this->activeThreadsLock);
			running = true; //in case the just mentioned situation happend, it is better to recheck
		}
		this->activeThreadsLock.Unlock();
	}
}
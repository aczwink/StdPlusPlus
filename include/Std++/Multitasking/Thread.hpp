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
#include "../Definitions.h"
#include "../Function.hpp"
#include <Std++/Natural.hpp>

namespace StdXX
{
    typedef int32(*ThreadFunction)();

	/**
	 * This is not an auto-start thread implementation like std::thread because it has a serious issue that is not that obvious.
	 * When using std::thread as a member of a class or when subclassing it, a critial error may occur.
	 *
	 * Example that demonstrates the problem:
	 * Say we have class A with member std::thread. Now when the constructor of A is called, std::thread() is called
	 * and the thread begins executing, while the constructor of A has not yet finished.
	 * Other members of A like mutexes or other variables, or maybe either the vtable might not yet be initialized.
	 */
    class STDPLUSPLUS_API Thread
    {
    public:
        //Constructors
        inline Thread() : systemHandle(nullptr), isAlive(false), function(nullptr)
		{
		}

        inline Thread(ThreadFunction func) : systemHandle(nullptr), isAlive(false), function(func)
		{
		}

		inline Thread(const Function<int32()> &function) : systemHandle(nullptr), isAlive(false), function(nullptr), functor(function)
		{
		}

        //Destructor
        virtual ~Thread();

        //Methods
		/*
		 * Waits for this thread at least 'duration' nanoseconds to terminate.
		 * This call blocks until the thread terminated or the timeout elapsed.
		*/
        void Join(uint64 duration = Natural<uint64>::Max());
		void Start();

		//Inline
		inline void Join_ms(uint64 ms)
		{
			this->Join(ms * 1000 * 1000);
		}

		/**
		 * Returns true if the thread has been started and has not yet terminated.
		 * Returns false otherwise.
		 * @return
		 */
		inline bool IsAlive() const
		{
			return this->isAlive;
		}

	private:
        //Members
        void *systemHandle;
		bool isAlive;
		ThreadFunction function;
		Function<int32()> functor;

		//Overrideable
		virtual void Run();

        //Methods
        int32 ThreadMain();
    };
}
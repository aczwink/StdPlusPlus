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
#include "../Definitions.h"

namespace StdXX
{
    //Move declarations
    class CConditionVariable;

    class STDPLUSPLUS_API Mutex
    {
        friend class ConditionVariable;

    public:
        //Constructors
        Mutex();

        Mutex(const Mutex&) = delete;
		Mutex(Mutex&&) = delete;

        //Destructor
        ~Mutex();

        //Methods
        void Lock();
        bool TryLock();
        void Unlock();

    private:
        //Members
        void *systemHandle;
    };

    class AutoLock
    {
	public:
		//Constructor
		inline AutoLock(Mutex &mutex) : mutex(mutex)
		{
			this->mutex.Lock();
		}

		//Destructor
		~AutoLock()
		{
			this->mutex.Unlock();
		}

	private:
		//Members
		Mutex &mutex;
    };
}
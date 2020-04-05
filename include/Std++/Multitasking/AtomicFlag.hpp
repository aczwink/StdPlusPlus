/*
 * Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/__Globaldependencies.h>

namespace StdXX
{
	class AtomicFlag
	{
#ifdef XPC_COMPILER_CLANG
	public:
		//Constructor
		inline AtomicFlag() : native(false)
		{
		}

		//Inline
		inline void Clear()
		{
			__c11_atomic_store(&this->native, false, std::memory_order_release);
		}

		inline bool TestAndSet()
		{
			return __c11_atomic_exchange(&this->native, true, std::memory_order_acquire);
		}
	private:
		//Members
		_Atomic(bool) native;
#endif

#ifdef XPC_COMPILER_GCC
    public:
        //Inline
        inline void Clear()
        {
            NOT_IMPLEMENTED_ERROR;
        }

        inline bool TestAndSet()
        {
            NOT_IMPLEMENTED_ERROR;
            return false;
        }
#endif
#ifdef XPC_COMPILER_MSVC
	public:
		//Constructor
		inline AtomicFlag() : native(0)
		{
		}

		//Inline
		inline void Clear()
		{
			_Compiler_barrier();
			volatile unsigned long* target = (volatile unsigned long*)&this->native;
			*target = 0;
		}

		inline bool TestAndSet()
		{
			return _interlockedbittestandset(&this->native, 0) != 0;
		}

	private:
		//Members
		long native;
#endif
	};
}
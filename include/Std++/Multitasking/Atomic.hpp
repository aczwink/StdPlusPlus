/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/__Globaldependencies.h>

namespace StdXX
{
	template <typename T>
	class Atomic
	{
	public:
		//Constructor
		inline Atomic()
		{
		}

#ifdef XPC_COMPILER_CLANG
		//Operators
		inline Atomic& operator=(T v) noexcept
		{
			__c11_atomic_store(&this->native, v, std::memory_order_seq_cst);
			return *this;
		}

		inline T operator++(int) //Postfix
		{
			return this->FetchAdd(1);
		}

		inline T operator--() //Prefix
		{
			return this->FetchSub(1) - 1;
		}

		//Inline
		inline T FetchAdd(T operand)
		{
			return __c11_atomic_fetch_add(&this->native, operand, std::memory_order_seq_cst);
		}

		inline T FetchSub(T operand)
		{
			return __c11_atomic_fetch_sub(&this->native, operand, std::memory_order_seq_cst);
		}

	private:
		//Members
		mutable _Atomic(T) native;
#endif
	};
}
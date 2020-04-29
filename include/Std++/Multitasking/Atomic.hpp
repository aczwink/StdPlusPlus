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
//Local
#include <Std++/__Globaldependencies.h>
#include <Std++/Definitions.h>
#include <Std++/Debug.hpp>

namespace StdXX
{
#ifdef XPC_COMPILER_CLANG
	template <typename T>
	class Atomic
	{
	public:
		//Constructor
		Atomic() = default;

		//Operators
		inline Atomic& operator=(T v) noexcept
		{
			__c11_atomic_store(&this->native, v, std::memory_order_seq_cst);
			return *this;
		}

		inline T operator*()
		{
			return __c11_atomic_load(&this->native, std::memory_order_seq_cst);
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
	};
#endif

#ifdef XPC_COMPILER_GCC
    template <typename T>
    class Atomic
    {
    public:
	    //Constructors
	    Atomic() = default;
	    constexpr Atomic(T value) noexcept : native(value)
	    {
	    }

        //Operators
        inline Atomic& operator=(T v) noexcept
        {
			__atomic_store_n(&this->native, v, std::memory_order_seq_cst);
            return *this;
        }

	    inline T operator*() const
	    {
		    return __atomic_load_n(&this->native, std::memory_order_seq_cst);
	    }

        inline T operator++(int) //Postfix
        {
            return this->FetchAdd(1);
        }

        inline T operator--(int) noexcept //Postfix
        {
	    	return this->FetchSub(1);
        }

        inline T operator--() //Prefix
        {
			return __atomic_sub_fetch(&this->native, 1, std::memory_order_seq_cst);
        }

		inline operator T() const noexcept
		{
	    	return this->operator*();
		}

		//Inline
		inline T FetchAdd(T operand)
		{
			return __atomic_fetch_add(&this->native, operand, std::memory_order_seq_cst);
		}

		inline T FetchSub(T operand) noexcept
		{
	    	return __atomic_fetch_sub(&this->native, operand, std::memory_order_seq_cst);
		}

    private:
    	//Members
		alignas(sizeof(T)) T native;
    };
#endif

#ifdef XPC_COMPILER_MSVC
	template <typename T>
	class Atomic
	{
	};

	template<>
	class Atomic<uint32>
	{
	public:
		//Operators
		inline Atomic& operator=(uint32 v) noexcept
		{
			_InterlockedExchange((volatile long*)&this->value, v);
			return *this;
		}

		inline uint32 operator++(int) //Postfix
		{
			return this->FetchAdd(1);
		}

		inline uint32 operator--() //Prefix
		{
			return this->FetchSub(1) - 1;
		}

		//Inline
		inline uint32 FetchAdd(uint32 operand)
		{
			return _InterlockedExchangeAdd((volatile long*)&this->value, operand);
		}

		inline uint32 FetchSub(uint32 operand)
		{
			return _InterlockedExchangeAdd((volatile long*)&this->value, 0 - operand);
		}

	private:
		//Members
		alignas(4) uint32 value;
	};
#endif
}
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
#pragma once
//Local
#include <Std++/Definitions.h>
#include <Std++/Debug.hpp>

namespace StdXX
{
	template<typename ValueType>
	class SharedPointer
	{
	public:
		//Constructors
		inline SharedPointer() : ptr(nullptr), refCount(nullptr)
		{
		}

		inline SharedPointer(ValueType* pointer) : ptr(pointer)
		{
			this->refCount = new uint32(1);
		}

		inline SharedPointer(const SharedPointer<ValueType>& other) : ptr(other.ptr), refCount(other.refCount)
		{
			if (this->refCount)
				(*this->refCount)++;
		}

		inline SharedPointer(SharedPointer<ValueType>&& other) : ptr(other.ptr), refCount(other.refCount)
		{
			other.ptr = nullptr;
			other.refCount = nullptr;
		}

		//Destructor
		inline ~SharedPointer()
		{
			this->Release();
		}

		//Operators
		inline ValueType &operator*()
		{
			ASSERT(this->ptr, u8"Can't dereference nullptr");
			return *this->ptr;
		}

		inline const ValueType &operator*() const
		{
			ASSERT(this->ptr, u8"Can't dereference nullptr");
			return *this->ptr;
		}

		inline ValueType* operator->()
		{
			ASSERT(this->ptr, u8"Can't access nullptr");
			return this->ptr;
		}

		inline const ValueType* operator->() const
		{
			ASSERT(this->ptr, u8"Can't access nullptr");
			return this->ptr;
		}

		inline SharedPointer<ValueType>& operator=(const SharedPointer<ValueType>& other)
		{
			this->Release();
			
			this->ptr = other.ptr;
			this->refCount = other.refCount;
			if (this->refCount)
				(*this->refCount)++;

			return *this;
		}

		inline SharedPointer<ValueType>& operator=(SharedPointer<ValueType>&& other)
		{
			this->Release();

			this->ptr = other.ptr;
			this->refCount = other.refCount;
			other.ptr = nullptr;
			other.refCount = nullptr;

			return *this;
		}

		inline bool operator!=(const SharedPointer<ValueType>& rhs) const
		{
			return this->ptr != rhs.ptr;
		}

		//Inline
		inline bool IsNull() const
		{
			return this->ptr == nullptr;
		}

		inline void Release()
		{
			if (this->refCount && (--(*this->refCount) == 0))
			{
				delete this->ptr;
				delete this->refCount;
				this->ptr = nullptr;
				this->refCount = nullptr;
			}
		}

	private:
		//Members
		ValueType* ptr;
		uint32* refCount;
	};
}
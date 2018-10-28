/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include "Std++/Debug.hpp"
#include "../Deleters.hpp"

namespace StdXX
{
	/**
	 * A single-owned non-shared pointer.
	 *
	 * @tparam ValueType
	 */
	template<typename ValueType>
	class UniquePointer
	{
	public:
		//Constructors
		inline UniquePointer() : pointer(nullptr)
		{
		}

		inline UniquePointer(ValueType *pointer) : pointer(pointer)
		{
		}

		UniquePointer(const UniquePointer &rhs) = delete; //Copy constructor

		UniquePointer(UniquePointer &&rhs) //Move ctor
		{
			this->pointer = rhs.pointer;
			rhs.pointer = nullptr;
		}

		//Destructor
		inline ~UniquePointer()
		{
			if(this->pointer)
				delete (this->pointer);
		}

		//Operators
		UniquePointer &operator=(const UniquePointer &rhs) = delete; //Copy assign

		inline UniquePointer &operator=(UniquePointer &&rhs) //Move assign
		{
			if(this->pointer)
				delete (this->pointer);
			this->pointer = rhs.pointer;
			rhs.pointer = nullptr;

			return *this;
		}

		inline ValueType &operator*()
		{
			ASSERT(this->pointer, u8"Can't dereference nullptr");
			return *this->pointer;
		}

		inline const ValueType &operator*() const
		{
			ASSERT(this->pointer, u8"Can't dereference nullptr");
			return *this->pointer;
		}

		inline ValueType *operator->()
		{
			ASSERT(this->pointer, u8"Can't access nullptr");
			return this->pointer;
		}

		inline const ValueType *operator->() const
		{
			ASSERT(this->pointer, u8"Can't access nullptr");
			return this->pointer;
		}

		//Logical operators
		inline bool operator==(const UniquePointer<ValueType>& other) const
		{
			return this->pointer == other.pointer;
		}

		inline bool operator!=(const UniquePointer<ValueType>& other) const
		{
			return this->pointer != other.pointer;
		}

		//Inline
		inline bool IsNull() const
		{
			return this->pointer == nullptr;
		}

	private:
		//Members
		ValueType *pointer;
	};
}
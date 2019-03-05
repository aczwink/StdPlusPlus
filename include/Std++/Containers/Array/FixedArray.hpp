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
#include <Std++/Debug.hpp>
#include <Std++/Type.hpp>
#include <Std++/Containers/Container.hpp>
#include <Std++/Containers/BinaryTreeSet/BinaryTreeSet.hpp>
#include "ArrayIterator.hpp"
#include "ConstArrayIterator.hpp"

namespace StdXX
{
	/**
	 * This container allocates, on construction, storage for a fixed number of elements.
	 * It can then NOT be resized anymore and is therefore fixed.
	 *
	 * @tparam DataType
	 */
	template<typename DataType>
	class FixedArray : public Container
	{
	public:
		//Constructors
		inline FixedArray(uint32 nElements)
		{
			this->nElements = nElements;
			this->data = new DataType[nElements];
		}

		template<uint32 sourceCount>
		inline FixedArray(const DataType (&source)[sourceCount])
		{
			this->nElements = sourceCount;
			this->data = new DataType[this->nElements];
			for(uint32 i = 0; i < sourceCount; i++)
				this->data[i] = source[i];
		}

		inline FixedArray(const FixedArray<DataType> &source) //copy ctor
		{
			*this = source;
		}

		inline FixedArray(FixedArray<DataType> &&source) //move ctor
		{
			this->data = nullptr;
			*this = Move(source);
		}

		//Destructor
		~FixedArray()
		{
			if(this->data)
				delete[] this->data;
		}

		//Operators
		FixedArray<DataType> &operator=(const FixedArray<DataType> &source) //copy assign
		{
			ASSERT(this->nElements == source.nElements, u8"Can't assign FixedArray when right side has a different number of elemens");

			for(uint32 i = 0; i < source.GetNumberOfElements(); i++)
			{
				this->data[i] = source[i];
			}

			return *this;
		}

		FixedArray<DataType> &operator=(FixedArray<DataType> &&source) //move assign
		{
			if(this->data)
				delete[] this->data;

			this->nElements = source.nElements;
			this->data = source.data;

			source.data = nullptr;

			return *this;
		}

		inline DataType &operator[](uint32 index)
		{
			ASSERT(index < this->nElements, u8"Index out of bounds");

			return this->data[index];
		}

		inline const DataType &operator[](uint32 index) const
		{
			ASSERT(index < this->nElements, u8"Index out of bounds");

			return this->data[index];
		}

		//Logical operators
		inline bool operator==(const FixedArray<DataType>& rhs) const
		{
			if(this->nElements != rhs.nElements)
				return false;
			for(uint32 i = 0; i < this->nElements; i++)
			{
				if(this->data[i] != rhs[i])
					return false;
			}
			return true;
		}

		//Inline
		inline DataType &Last()
		{
			ASSERT(!this->IsEmpty(), u8"Can't get last element from empty array.");
			return this->data[this->nElements - 1];
		}

		inline const DataType &Last() const
		{
			ASSERT(!this->IsEmpty(), u8"Can't get last element from empty array.");
			return this->data[this->nElements - 1];
		}

		//For range-based loops
		inline ArrayIterator<FixedArray, DataType> begin()
		{
			return ArrayIterator<FixedArray, DataType>(*this);
		}

		inline ConstArrayIterator<FixedArray, DataType> begin() const
		{
			return ConstArrayIterator<FixedArray, DataType>(*this);
		}

		inline ArrayIterator<FixedArray, DataType> end()
		{
			return ArrayIterator<FixedArray, DataType>(*this, this->nElements);
		}

		inline ConstArrayIterator<FixedArray, DataType> end() const
		{
			return ConstArrayIterator<FixedArray, DataType>(*this, this->nElements);
		}

	private:
		//Members
		DataType *data;
	};
}
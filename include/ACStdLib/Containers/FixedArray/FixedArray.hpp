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
#pragma once
//Local
#include "../../Debug.h"
#include "../../Type.hpp"
#include "../Container.hpp"

namespace ACStdLib
{
	/**
	 * This container allocates on construction storage for a fixed number of elements.
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

		inline FixedArray(const FixedArray<DataType> &source) //copy ctor
		{
			*this = source;
		}

		inline FixedArray(FixedArray<DataType> &&source) //move ctor
		{
			this->data = nullptr;
			*this = Forward(source);
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
			ASSERT(this->nElements == source.nElements);

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
			ASSERT(index < this->nElements);

			return this->data[index];
		}

		inline const DataType &operator[](uint32 index) const
		{
			ASSERT(index < this->nElements);

			return this->data[index];
		}

	private:
		//Members
		DataType *data;
	};
}
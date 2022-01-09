/*
* Copyright (c) 2018,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Container.hpp>

namespace StdXX
{
	/**
	* This container allocates, on construction, storage for a fixed number of elements.
	* It can then NOT be resized anymore and is therefore fixed.
	*
	* @tparam DataType
	*/
	template<typename DataType>
	class FixedTable : public Container
	{
	public:
		//Constructors
		inline FixedTable(uint32 nRows, uint32 nCols) : nRows(nRows), nCols(nCols), data(new DataType[nRows * nCols])
		{
			this->nElements = nRows * nCols;
		}

		inline FixedTable(const FixedTable<DataType> &source) //copy ctor
		{
		    this->nRows = source.nRows;
		    this->nCols = source.nCols;
		    this->data = new DataType[source.nElements];
		    this->nElements = source.nElements;
			*this = source;
		}

		inline FixedTable(FixedTable<DataType> &&source) //move ctor
		{
			this->data = nullptr;
			*this = Move(source);
		}

		//Destructor
		~FixedTable()
		{
			delete[] this->data;
		}

		//Operators
		FixedTable<DataType> &operator=(const FixedTable<DataType> &source) //copy assign
		{
			ASSERT(this->nElements == source.nElements, u8"Can't assign FixedTable when right side has a different number of elemens");
			for (uint32 i = 0; i < source.GetNumberOfElements(); i++)
				this->data[i] = source.data[i];
			
			return *this;
		}

		FixedTable<DataType> &operator=(FixedTable<DataType> &&source) //move assign
		{
			delete[] this->data;

			this->nRows = source.nRows;
			this->nCols = source.nCols;
			this->nElements = source.nElements;
			this->data = source.data;

			source.data = nullptr;

			return *this;
		}

		inline DataType &operator()(uint32 row, uint32 col)
		{
			ASSERT((row * this->nCols + col) < this->nElements, u8"Index out of bounds");

			return this->data[(row * this->nCols + col)];
		}

		inline const DataType &operator()(uint32 row, uint32 col) const
		{
			ASSERT((row * this->nCols + col) < this->nElements, u8"Index out of bounds");

			return this->data[(row * this->nCols + col)];
		}

		//Inline
		inline uint32 GetNumberOfColumns() const
		{
			return this->nCols;
		}

		inline uint32 GetNumberOfRows() const
		{
			return this->nRows;
		}

	private:
		//Members
		uint32 nRows;
		uint32 nCols;
		DataType * data;
	};
}
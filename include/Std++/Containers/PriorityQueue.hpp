/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include "ResizeableSequenceContainer.hpp"

namespace StdXX
{
	/**
	 * Implementation as binary heap.
	 * @tparam DataType
	 * @tparam Comparator
	 */
    template<typename DataType, typename Comparator = LessThan<DataType>>
    class PriorityQueue : public ResizeableSequenceContainer<DataType>
    {
    public:
        //Methods
    	void Insert(const DataType& value)
		{
			this->EnsureAdditionalCapacity(1);
			this->data[this->nElements] = value;

			this->HeapifyUpwards(this->nElements++);
		}

        void Insert(DataType&& value)
        {
			this->EnsureAdditionalCapacity(1);
			this->data[this->nElements] = Move(value);

			this->HeapifyUpwards(this->nElements++);
        }

        DataType PopTop()
        {
			ASSERT(!this->IsEmpty(), u8"Can't get top element of empty priority queue.");

			DataType root = Move(this->data[0]);
			this->data[0] = Move(this->data[--this->nElements]);
			this->Heapify(0);

			return root;
        }

        void Remove(const DataType& dataType)
		{
        	for(uint32 i = 0; i < this->nElements; i++)
			{
        		if(this->data[i] == dataType)
				{
        			this->data[i] = Move(this->data[--this->nElements]);
        			this->Heapify(i);
        			break;
				}
			}
		}

        //Inline
		inline const DataType& Top() const
		{
			ASSERT(!this->IsEmpty(), u8"Can't get top element of empty priority queue.");
			return this->data[0];
		}

	private:
    	//Members
    	Comparator comparator;

        //Methods
		void Heapify(uint32 index)
		{
			uint32 largest = index;

			if( (this->LeftChildIndex(index) < this->nElements) and this->comparator(this->data[largest], this->data[this->LeftChildIndex(index)]))
				largest = this->LeftChildIndex(index);
			if( (this->RightChildIndex(index) < this->nElements) and this->comparator(this->data[largest], this->data[this->RightChildIndex(index)]))
				largest = this->RightChildIndex(index);

			if(largest != index)
			{
				Swap(this->data[index], this->data[largest]);
				this->Heapify(largest);
			}
		}

		void HeapifyUpwards(uint32 index)
		{
			//restore heap property
			while( (index > 0) and this->comparator(this->data[this->ParentIndex(index)], this->data[index]) )
			{
				Swap(this->data[index], this->data[this->ParentIndex(index)]);
				index = this->ParentIndex(index);
			}
		}

        //Inline
		inline uint32 LeftChildIndex(uint32 idx) const
		{
			return 2 * idx + 1;
		}

		inline uint32 RightChildIndex(uint32 idx) const
		{
			return 2 * idx + 2;
		}

		inline uint32 ParentIndex(uint32 index) const
		{
			return (index - 1) / 2;
		}
    };
}
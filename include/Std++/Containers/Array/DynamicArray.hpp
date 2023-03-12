/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Unsigned.hpp>
#include <Std++/Containers/BinaryTreeSet/BinaryTreeSet.hpp>
#include <Std++/Containers/ResizeableSequenceContainer.hpp>
#include <Std++/Utility.hpp>
#include "ArrayIterator.hpp"
#include "ConstArrayIterator.hpp"

namespace StdXX
{
    template<typename DataType>
    class DynamicArray : public ResizeableSequenceContainer<DataType>
    {
        //friend class DynamicArray;
    public:
        //Constructors
        inline DynamicArray() = default;

		inline DynamicArray(const BinaryTreeSet<DataType>& binaryTreeSet)
		{
			this->Resize(binaryTreeSet.GetNumberOfElements());
			uint32 i = 0;
			for(const DataType& v : binaryTreeSet)
				this->data[i++] = v;
		}

        inline DynamicArray(const DynamicArray<DataType> &refSource) //copy ctor
        {
            *this = refSource;
        }

        inline DynamicArray(DynamicArray<DataType> &&refSource) //move ctor
        {
            *this = Move(refSource);
        }

        //Operators
		DynamicArray<DataType> &operator=(const DynamicArray<DataType> &rhs) //copy assign
		{
			ResizeableSequenceContainer<DataType>::operator=(rhs);
			return *this;
		}

		DynamicArray<DataType> &operator=(DynamicArray<DataType> &&rhs) //move assign
		{
			ResizeableSequenceContainer<DataType>::operator=(Move(rhs));
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
		inline bool operator==(const DynamicArray<DataType>& other) const
		{
			if (this->nElements != other.nElements)
				return false;
			
			for (uint32 i = 0; i < this->nElements; i++)
			{
				if (this->data[i] != other.data[i])
					return false;
			}
			return true;
		}

		inline bool operator!=(const DynamicArray<DataType>& other) const
		{
			return !(*this == other);
		}

        //Methods
        template<typename OutType>
        DynamicArray<OutType> Cast() const
        {
            DynamicArray<OutType> outArr;

            outArr.EnsureCapacity(this->nElements);
            for(uint32 i = 0; i < this->nElements; i++)
            {
                outArr[i] = (OutType)this->operator[](i);
            }
            outArr.nElements = this->nElements;
            outArr.elementsAllocInterval = this->elementsAllocInterval;

            return outArr;
        }

        inline void Clear()
        {
            this->nElements = 0;
        }

        int32 Find(const DataType &refValue) const
        {
            for(uint32 i = 0; i < this->nElements; i++)
            {
                if(this->data[i] == refValue)
                    return i;
            }
            return Unsigned<uint32>::Max();
        }

        inline bool IsInArray(const DataType &refValue) const
        {
            return this->Find(refValue) != Unsigned<uint32>::Max();
        }

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

        DataType Pop()
        {
            this->nElements--;
            DataType data = Move(this->data[this->nElements]);

            return data;
        }

        uint32 Push(const DataType &refValue)
        {
            this->EnsureAdditionalCapacity(1);

            this->data[this->nElements] = refValue;

            return this->nElements++;
        }

		uint32 Push(DataType&& value)
		{
			this->EnsureAdditionalCapacity(1);
			this->data[this->nElements] = Forward<DataType>(value);

			return this->nElements++;
		}

		void Remove(uint32 index)
		{
			ASSERT(index < this->nElements, u8"Index out of bounds");

			for(uint32 i = index + 1; i < this->GetNumberOfElements(); i++)
				this->data[i-1] = Move(this->data[i]);
			this->Resize(this->GetNumberOfElements() - 1);
		}

        inline void Resize(uint32 nElements)
        {
            this->EnsureCapacity(nElements);
            this->nElements = nElements;
        }

        void Sort()
        {
            uint32 min;
            DataType tmp;

            for(uint32 i = 0; i < this->nElements; i++)
            {
                min = i;
                for(uint32 j = i + 1; j < this->nElements; j++)
                {
                    if(this->data[j] < this->data[min])
                    {
                        min = j;
                    }
                }
                tmp = this->data[i];
                this->data[i] = this->data[min];
                this->data[min] = tmp;
            }
        }

        //For range-based loops
        inline ArrayIterator<DynamicArray, DataType> begin()
        {
            return ArrayIterator<DynamicArray, DataType>(*this);
        }

        inline ConstArrayIterator<DynamicArray, DataType> begin() const
        {
            return ConstArrayIterator<DynamicArray, DataType>(*this);
        }

        inline ArrayIterator<DynamicArray, DataType> end()
        {
            return ArrayIterator<DynamicArray, DataType>(*this, this->nElements);
        }

        inline ConstArrayIterator<DynamicArray, DataType> end() const
        {
            return ConstArrayIterator<DynamicArray, DataType>(*this, this->nElements);
        }
    };
}
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
#include "../AResizeableContainer.h"
#include "DynamicArrayIterator.hpp"
#include "DynamicArrayConstIterator.hpp"

namespace ACStdLib
{
    template<typename DataType>
    class DynamicArray : public AResizeableContainer<DataType>
    {
        //friend class DynamicArray;
    public:
        //Constructors
        inline DynamicArray()
        {
            this->SetAllocationInterval(0);
        }

        inline DynamicArray(const DynamicArray<DataType> &refSource) //copy ctor
        {
            *this = refSource;
        }

        inline DynamicArray(DynamicArray<DataType> &&refSource) //move ctor
        {
            *this = refSource;
        }

        //Operators
        DynamicArray<DataType> &operator=(const DynamicArray<DataType> &refSource) //copy assign
        {
            this->Release();
            this->EnsureCapacity(refSource.nElements);

            for(uint32 i = 0; i < refSource.GetNumberOfElements(); i++)
            {
                this->pData[i] = refSource[i];
            }
            this->nElements = refSource.nElements;
            this->elementsAllocInterval = refSource.elementsAllocInterval;

            return *this;
        }

        DynamicArray<DataType> &operator=(DynamicArray<DataType> &&refSource) //move assign
        {
            this->Release();

            this->capacity = refSource.capacity;
            this->elementsAllocInterval = refSource.elementsAllocInterval;
            this->pData = refSource.pData;
            this->nElements = refSource.nElements;

            refSource.pData = nullptr;

            return *this;
        }

        inline DataType &operator[](uint32 index)
        {
            ASSERT(index < this->nElements);

            return this->pData[index];
        }

        inline const DataType &operator[](uint32 index) const
        {
            ASSERT(index < this->nElements);

            return this->pData[index];
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
                if(this->pData[i] == refValue)
                    return i;
            }
            return UINT32_MAX;
        }

        inline bool IsInArray(const DataType &refValue) const
        {
            return this->Find(refValue) != UINT32_MAX;
        }

        DataType Pop()
        {
            DataType data;

            this->nElements--;
            data = this->pData[this->nElements];

            return data;
        }

        uint32 Push(const DataType &refValue)
        {
            this->EnsureAdditionalCapacity(1);

            this->pData[this->nElements] = refValue;

            return this->nElements++;
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
                    if(this->pData[j] < this->pData[min])
                    {
                        min = j;
                    }
                }
                tmp = this->pData[i];
                this->pData[i] = this->pData[min];
                this->pData[min] = tmp;
            }
        }

        //For range-based loops
        inline DynamicArrayIterator<DataType> begin()
        {
            return DynamicArrayIterator<DataType>(*this);
        }

        inline DynamicArrayConstIterator<DataType> begin() const
        {
            return DynamicArrayConstIterator<DataType>(*this);
        }

        inline DynamicArrayIterator<DataType> end()
        {
            return DynamicArrayIterator<DataType>(*this, this->nElements);
        }

        inline DynamicArrayConstIterator<DataType> end() const
        {
            return DynamicArrayConstIterator<DataType>(*this, this->nElements);
        }
    };
}
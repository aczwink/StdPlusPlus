/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Memory.h>
#include "../Type.hpp"
#include "Container.hpp"

namespace StdPlusPlus
{
    template<typename DataType>
    class ResizeableSequenceContainer : public Container
    {
    public:
        //Constructor
        ResizeableSequenceContainer()
        {
            this->elementsAllocInterval = 0;
            this->capacity = 0;
            this->data = nullptr;
        }

        //Destructor
        virtual ~ResizeableSequenceContainer()
        {
            if(this->data)
				this->FreeContainer();
        }

        //Overwriteable
        virtual void EnsureCapacity(uint32 requiredNumberOfElements)
        {
            if(this->capacity < requiredNumberOfElements)
				this->AllocateContainer(requiredNumberOfElements);
		}

        virtual void Release()
        {
            if(this->data)
            {
				this->FreeContainer();
                this->data = nullptr;
            }
            this->capacity = 0;
            this->nElements = 0;
        }

        //Inline
        inline void EnsureAdditionalCapacity(uint32 requiredAdditionalCapacity)
        {
            this->EnsureCapacity(this->nElements + requiredAdditionalCapacity);
        }

        inline uint32 GetCapacity() const
        {
            return this->capacity;
        }

        inline uint32 GetSize() const
        {
            return this->nElements * sizeof(DataType);
        }

        inline void SetAllocationInterval(uint32 nElementsPerInterval)
        {
            this->elementsAllocInterval = nElementsPerInterval;
        }

    protected:
        //Members
        DataType *data;
        uint32 capacity; //shouldn't be altered other through methods in this class, but needed for move ctor
        uint32 elementsAllocInterval; //shouldn't be altered other through methods in this class, but needed for move and copy ctor

        //Inline
        inline const DataType *GetEnd() const //pointer to first element after the container, shouldn't read there
        {
            return this->data + this->capacity;
        }

	private:
		//Inline
		template <typename T = DataType>
		typename EnableIf<IsTrivial<T>::value, void>::type
		inline AllocateContainer(uint32 requiredNumberOfElements)
		{
			uint32 newElementsCount = this->GetAllocationElementsCount(requiredNumberOfElements);
			DataType *pNewBuffer = (DataType *)MemRealloc(this->data, newElementsCount * sizeof(DataType));
			ASSERT(pNewBuffer, u8"If you see this, report it to StdPlusPlus");

			this->data = pNewBuffer;
			this->capacity = newElementsCount;
		}

		template <typename T = DataType>
		typename EnableIf<!IsTrivial<T>::value, void>::type
		inline AllocateContainer(uint32 requiredNumberOfElements)
		{
			uint32 newElementsCount = this->GetAllocationElementsCount(requiredNumberOfElements);
			DataType *newBuffer = new DataType[newElementsCount];
			ASSERT(newBuffer, u8"If you see this, report it to StdPlusPlus");

			for(uint32 i = 0; i < this->nElements; i++)
				newBuffer[i] = this->data[i];

			delete[] this->data;
			this->data = newBuffer;
			this->capacity = newElementsCount;
		}

		template <typename T = DataType>
		typename EnableIf<IsTrivial<T>::value, void>::type
		inline FreeContainer()
		{
			MemFree(this->data);
		};

		template <typename T = DataType>
		typename EnableIf<!IsTrivial<T>::value, void>::type
		inline FreeContainer()
		{
			delete[] this->data;
		};

		inline uint32 GetAllocationElementsCount(uint32 requiredNumberOfElements)
		{
			if(this->elementsAllocInterval == 0)
			{
				uint32 newNElements = 2 * this->capacity;
				if(newNElements < requiredNumberOfElements)
					newNElements = requiredNumberOfElements;
				return newNElements;
			}

			uint32 nAllocationIntervals = requiredNumberOfElements / this->elementsAllocInterval;
			if(requiredNumberOfElements % this->elementsAllocInterval)
				nAllocationIntervals++;

			return nAllocationIntervals * this->elementsAllocInterval;
		}
    };
}
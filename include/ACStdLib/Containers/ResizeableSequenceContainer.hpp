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
#include "../Debug.h"
#include "../Memory.h"
#include "Container.hpp"

namespace ACStdLib
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
            this->data = NULL;
        }

        //Destructor
        virtual ~ResizeableSequenceContainer()
        {
            if(this->data)
                MemFree(this->data);
        }

        //Overwriteable
        virtual void EnsureCapacity(uint32 requiredNumberOfElements)
        {
            if(this->capacity < requiredNumberOfElements)
            {
                uint32 newNElements;
                DataType *pNewBuffer;

                if(this->elementsAllocInterval == 0)
                {
                    newNElements = 2 * this->capacity;
                    if(newNElements < requiredNumberOfElements)
                        newNElements = requiredNumberOfElements;
                }
                else
                {
                    uint32 nAllocationIntervals;

                    nAllocationIntervals = requiredNumberOfElements / this->elementsAllocInterval;
                    if(requiredNumberOfElements % this->elementsAllocInterval)
                        nAllocationIntervals++;

                    newNElements = nAllocationIntervals * this->elementsAllocInterval;
                }

                pNewBuffer = (DataType *)MemRealloc(this->data, newNElements * sizeof(DataType));
                ASSERT(pNewBuffer, "If you see this, report it to ACStdLib");

                this->data = pNewBuffer;
                this->capacity = newNElements;
            }
        }

        virtual void Release()
        {
            if(this->data)
            {
                MemFree(this->data);
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
    };
}
/*
 * Copyright (c) 2017-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Memory.hpp>
#include <Std++/Utility.hpp>
#include <Std++/Type.hpp>
#include <Std++/Mathematics.hpp>
#include <Std++/Signed.hpp>
#include <Std++/Unsigned.hpp>
#include "Container.hpp"

namespace StdXX
{
    template<typename DataType>
    class ResizeableSequenceContainer : public Container
    {
    public:
        //Constructor
        ResizeableSequenceContainer()
        {
            this->capacity = 0;
            this->data = nullptr;
        }

        //Destructor
        virtual ~ResizeableSequenceContainer()
        {
            if(this->data)
				this->FreeContainer();
        }

		//Operators
		ResizeableSequenceContainer<DataType> &operator=(const ResizeableSequenceContainer<DataType> &rhs) //copy assign
		{
			this->Release();
			this->EnsureCapacity(rhs.nElements);

			for (uint32 i = 0; i < rhs.GetNumberOfElements(); i++)
			{
				this->data[i] = rhs.data[i];
			}

			this->nElements = rhs.nElements;
			
			return *this;
		}

		ResizeableSequenceContainer<DataType> &operator=(ResizeableSequenceContainer<DataType> &&rhs) //move assign
		{
			this->Release();

			this->data = rhs.data;
			rhs.data = nullptr;

			this->capacity = rhs.capacity;
			rhs.capacity = 0;

			this->nElements = rhs.nElements;
			rhs.nElements = 0;

			return *this;
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

    protected:
        //Members
        DataType *data;

        //Inline
        inline const DataType *GetEnd() const //pointer to first element after the container, shouldn't read there
        {
            return this->data + this->capacity;
        }

	private:
		//Members
		uint32 capacity;

		//Inline
		template <typename T = DataType>
		typename Type::EnableIf<Type::IsTrivial<T>::value, void>::type
		inline AllocateContainer(uint32 requiredNumberOfElements)
		{
			uint32 newElementsCount = this->CalculateNewCapacity(requiredNumberOfElements);
			DataType *pNewBuffer = (DataType *)MemRealloc(this->data, newElementsCount * sizeof(DataType));
			ASSERT(pNewBuffer, u8"If you see this, report it to StdXX");

			this->data = pNewBuffer;
			this->capacity = newElementsCount;
		}

		template <typename T = DataType>
		typename Type::EnableIf<!Type::IsTrivial<T>::value, void>::type
		inline AllocateContainer(uint32 requiredNumberOfElements)
		{
			uint32 newElementsCount = this->CalculateNewCapacity(requiredNumberOfElements);
			DataType *newBuffer = new DataType[newElementsCount];
			ASSERT(newBuffer, u8"If you see this, report it to StdXX");

			for(uint32 i = 0; i < this->nElements; i++)
				newBuffer[i] = Move(this->data[i]);

			delete[] this->data;
			this->data = newBuffer;
			this->capacity = newElementsCount;
		}

		template <typename T = DataType>
		typename Type::EnableIf<Type::IsTrivial<T>::value, void>::type
		inline FreeContainer()
		{
			MemFree(this->data);
		};

		template <typename T = DataType>
		typename Type::EnableIf<!Type::IsTrivial<T>::value, void>::type
		inline FreeContainer()
		{
			delete[] this->data;
		};

		inline uint32 CalculateNewCapacity(uint32 requiredNumberOfElements)
		{
			uint32 newCapacity = Math::Max(this->capacity, 32_u32);
			if(requiredNumberOfElements >= Signed<int32>::Max())
				newCapacity = Unsigned<uint32>::Max();
			while(newCapacity < requiredNumberOfElements)
				newCapacity <<= 1;

			return newCapacity;
		}
    };
}
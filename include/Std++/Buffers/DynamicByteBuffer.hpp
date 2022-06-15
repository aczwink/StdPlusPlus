/*
* Copyright (c) 2021-2022 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/SeekableOutputStream.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#include <Std++/Streams/SeekableInputStream.hpp>
#include <Std++/Memory.hpp>
#include <Std++/Utility.hpp>

namespace StdXX
{
	class DynamicByteBuffer
	{
	public:
		//Constructors
		inline DynamicByteBuffer()
		{
			this->data = nullptr;
			this->size = 0;
			this->capacity = 0;
		}

		inline DynamicByteBuffer(const DynamicByteBuffer& buffer) : DynamicByteBuffer()
		{
			*this = buffer;
		}

		inline DynamicByteBuffer(DynamicByteBuffer&& buffer) : DynamicByteBuffer()
		{
			*this = Move(buffer);
		}

		//Destructor
		inline ~DynamicByteBuffer()
		{
			this->Release();
		}

		//Operators
		DynamicByteBuffer& operator=(const DynamicByteBuffer& buffer);
		DynamicByteBuffer& operator=(DynamicByteBuffer&& buffer);

		inline uint8 operator[](uint32 index) const
		{
			ASSERT(index < this->size, u8"Index out of bounds");

			return this->data[index];
		}

		inline bool operator==(const DynamicByteBuffer& other) const
		{
			if(this->size != other.size)
				return false;
			return MemCmp(this->data, other.data, this->size) == 0;
		}

		//Properties
		inline uint8* Data()
		{
			return this->data;
		}

		inline const uint8* Data() const
		{
			return this->data;
		}

		inline uint32 Size() const
		{
			return this->size;
		}

		//Methods
		UniquePointer<SeekableInputStream> CreateInputStream() const;
		UniquePointer<SeekableOutputStream> CreateOutputStream();

		//Inline
		inline void CopyFrom(const void* source, uint32 offset, uint32 size)
		{
			ASSERT(offset + size <= this->size, u8"Out of bounds");

			MemCopy(&this->data[offset], source, size);
		}

		inline void CopyTo(void* destination, uint32 offset, uint32 size) const
		{
			ASSERT(offset + size <= this->size, u8"Out of bounds");

			MemCopy(destination, &this->data[offset], size);
		}

		inline void Release()
		{
			if(this->data)
			{
				MemFree(this->data);
				this->data = nullptr;
			}
			this->size = 0;
			this->capacity = 0;
		}

		inline void Resize(uint32 size)
		{
			this->EnsureSize(size);
			this->size = size;
		}

	private:
		//Members
		uint8* data;
		uint32 size;
		uint32 capacity;

		//Methods
		void EnsureSize(uint32 size);
	};
}
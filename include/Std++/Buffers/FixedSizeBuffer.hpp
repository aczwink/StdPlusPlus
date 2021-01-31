/*
* Copyright (c) 2019-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Memory.hpp>
#include <Std++/Unsigned.hpp>
#include <Std++/Streams/Writers/DataWriter.hpp>
#include <Std++/Streams/BufferOutputStream.hpp>
#include <Std++/Utility.hpp>

namespace StdXX
{
	class FixedSizeBuffer
	{
	public:
		//Constructor
		inline FixedSizeBuffer(const void* source, uint32 size) : size(size)
		{
			this->data = (byte*)MemAlloc(size);
			this->CopyFrom(source, size);
		}

		inline FixedSizeBuffer(uint32 size) : size(size)
		{
			this->data = (byte*)MemAlloc(size);
		}

		inline FixedSizeBuffer(const FixedSizeBuffer& rhs) : size(rhs.size)
		{
			this->data = (byte*)MemAlloc(rhs.size);
			*this = rhs;
		}

		inline FixedSizeBuffer(FixedSizeBuffer&& rhs)
		{
			this->data = nullptr;
			*this = Move(rhs);
		}

		//Destructor
		inline ~FixedSizeBuffer()
		{
			if(this->data)
				MemFree(this->data);
		}

		//Operators
		inline FixedSizeBuffer& operator=(const FixedSizeBuffer& rhs)
		{
			ASSERT_EQUALS(this->size, rhs.size);
			MemCopy(this->data, rhs.data, this->size);

			return *this;
		}

		inline FixedSizeBuffer& operator=(FixedSizeBuffer&& rhs)
		{
			if(this->data)
				MemFree(this->data);

			this->data = rhs.data;
			this->size = rhs.size;
			rhs.data = nullptr;
			rhs.size = 0;

			return *this;
		}

		inline uint8& operator[](uint32 index)
		{
			ASSERT(index < this->size, u8"Index out of bounds");

			return this->data[index];
		}

		inline bool operator==(const FixedSizeBuffer& buffer) const
		{
			if(this->size != buffer.size)
				return false;
			return MemCmp(this->data, buffer.data, this->size) == 0;
		}

		inline bool operator!=(const FixedSizeBuffer& buffer) const
		{
			return !(*this == buffer);
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
		uint32 FindBytesReversed(const byte* bytes, uint32 size, uint32 startOffset = StdXX::Unsigned<uint32>::Max());

		//Inline
		inline void CopyFrom(const void* source, uint32 size)
		{
			ASSERT(size <= this->size, u8"Size too small");
			MemCopy(this->data, source, size);
		}

		inline uint32 FindReversedUInt32(uint32 value, bool bigEndian, uint32 startOffset = StdXX::Unsigned<uint32>::Max())
		{
			byte buffer[4];
			BufferOutputStream bufferOutputStream(buffer, sizeof(buffer));
			DataWriter dataWriter(bigEndian, bufferOutputStream);
			dataWriter.WriteUInt32(value);

			return this->FindBytesReversed(buffer, sizeof(buffer), startOffset);
		}

	private:
		//Members
		byte* data;
		uint32 size;
	};
}
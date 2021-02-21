/*
* Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
//Class header
#include <Std++/Buffers/DynamicByteBuffer.hpp>
//Local
#include <Std++/Mathematics.hpp>
#include <Std++/Signed.hpp>
//Namespaces
using namespace StdXX;

//Operators
DynamicByteBuffer &DynamicByteBuffer::operator=(const DynamicByteBuffer &buffer)
{
	this->Resize(buffer.size);
	MemCopy(this->data, buffer.data, buffer.size);

	return *this;
}

DynamicByteBuffer &DynamicByteBuffer::operator=(DynamicByteBuffer &&buffer)
{
	this->Release();

	this->data = buffer.data;
	this->size = buffer.size;
	this->capacity = buffer.capacity;

	buffer.data = nullptr;
	buffer.size = 0;
	buffer.capacity = 0;

	return *this;
}

//Public methods
UniquePointer<SeekableInputStream> DynamicByteBuffer::CreateInputStream() const
{
	class DynamicByteBufferInputStream : public SeekableInputStream
	{
	public:
		//Constructor
		inline DynamicByteBufferInputStream(const DynamicByteBuffer& buffer) : buffer(buffer)
		{
			this->offset = 0;
		}

		//Methods
		uint32 GetBytesAvailable() const override
		{
			NOT_IMPLEMENTED_ERROR;
			return 0;
		}

		bool IsAtEnd() const override
		{
			NOT_IMPLEMENTED_ERROR;
			return false;
		}

		uint32 ReadBytes(void *destination, uint32 count) override
		{
			uint32 offset = Math::Min(this->offset, this->buffer.Size());
			uint32 nBytesToCopy = Math::Min(count, this->buffer.Size() - offset);
			this->buffer.CopyTo(destination, offset, nBytesToCopy);
			this->offset += nBytesToCopy;

			return nBytesToCopy;
		}

		uint32 Skip(uint32 nBytes) override
		{
			NOT_IMPLEMENTED_ERROR;
			return 0;
		}

		uint64 QueryCurrentOffset() const override
		{
			NOT_IMPLEMENTED_ERROR;
			return 0;
		}

		void SeekTo(uint64 offset) override
		{
			NOT_IMPLEMENTED_ERROR;
		}

		uint64 QueryRemainingBytes() const override
		{
			NOT_IMPLEMENTED_ERROR;
			return 0;
		}

		uint64 QuerySize() const override
		{
			NOT_IMPLEMENTED_ERROR;
			return 0;
		}

	private:
		//Members
		uint32 offset;
		const DynamicByteBuffer& buffer;
	};

	return new DynamicByteBufferInputStream(*this);
}

UniquePointer<SeekableOutputStream> DynamicByteBuffer::CreateOutputStream()
{
	class DynamicByteBufferOutputStream : public SeekableOutputStream
	{
	public:
		//Constructor
		inline DynamicByteBufferOutputStream(DynamicByteBuffer& buffer) : buffer(buffer)
		{
			this->offset = 0;
		}

		//Methods
		void Flush() override
		{
			NOT_IMPLEMENTED_ERROR;
		}

		uint32 WriteBytes(const void *source, uint32 size) override
		{
			this->buffer.EnsureSize(this->offset + size);
			this->buffer.CopyFrom(source, this->offset, size);
			this->offset += size;
			return size;
		}

		uint64 QueryCurrentOffset() const override
		{
			return this->offset;
		}

		void SeekTo(uint64 offset) override
		{
			this->offset = Math::Min(Unsigned<uint32>::DowncastToClosest(offset), this->buffer.Size());
		}

	private:
		//Members
		uint32 offset;
		DynamicByteBuffer& buffer;
	};

	return new DynamicByteBufferOutputStream(*this);
}

//Private methods
void DynamicByteBuffer::EnsureSize(uint32 size)
{
	if(this->capacity >= size)
	{
		this->size = Math::Max(size, this->size);
		return;
	}

	uint32 newCapacity = Math::Max(this->capacity, 32_u32);
	if(size >= Signed<int32>::Max())
		newCapacity = Unsigned<uint32>::Max();
	while(newCapacity < size)
		newCapacity <<= 1;

	void* newData = MemRealloc(this->data, newCapacity);
	ASSERT(newData != nullptr, u8"Not enough memory");

	this->data = static_cast<uint8 *>(newData);
	this->size = size;
	this->capacity = newCapacity;

}
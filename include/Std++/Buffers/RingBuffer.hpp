/*
* Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/Memory.hpp>
#include <Std++/Mathematics.hpp>

namespace StdXX
{
	class RingBuffer
	{
	public:
		//Constructor
		inline RingBuffer(uint32 size) : size(size)
		{
			this->data = (byte*)MemAlloc(size);
			this->index = 0;
		}

		//Destructor
		inline ~RingBuffer()
		{
			MemFree(this->data);
		}

		//Properties
		inline const uint8* Data() const
		{
			return this->data;
		}

		//Methods
		/**
		 * Reads bytes from this buffer by going 'distance' bytes back from the current ring buffer position and then reading 'length' bytes forward.
		 * @param dest
		 * @param distance
		 * @param length
		 */
		void Read(void* destination, uint16 distance, uint16 length) const;

		//Inline
		inline void Append(byte b)
		{
			this->data[this->index++] = b;

			if(this->index == this->size)
				this->index = 0;
		}

		inline void Append(const void* source, uint32 length)
		{
			const uint8* src = static_cast<const uint8 *>(source);
			while(length--)
				this->Append(*src++);
		}

	protected:
		//Inline
		inline uint32 CalcBackOffsetIndex(uint32 backOffset) const
		{
			return (this->size + this->index - backOffset) % this->size;
		}

		inline uint32 CalcNumberOfBytesToRead(uint32 readIndex) const
		{
			//check all overlaps
			uint32 nBytesToRead = this->size - readIndex;
			nBytesToRead = Math::Min(nBytesToRead, this->size - this->index);
			nBytesToRead = Math::Min(nBytesToRead, this->index - readIndex); //we can not overtake what we didnt read yet

			return nBytesToRead;
		}

		inline void CopyToTail(uint32 readIndex, uint32 nBytesToWrite)
		{
			MemCopy(&this->data[this->index], &this->data[readIndex], nBytesToWrite);
			this->index = (this->index + nBytesToWrite) % this->size;
		}

	private:
		//Members
		uint8* data;
		uint32 size;
		uint32 index;
	};
}
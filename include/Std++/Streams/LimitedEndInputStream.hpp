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
#pragma once
//Local
#include <Std++/Mathematics.hpp>
#include <Std++/Memory.hpp>
#include "ReadOnlyInputStream.hpp"
#include "BufferedOutputStream.hpp"

namespace StdXX
{
	/**
	 * This class holds the last n bytes of a stream in buffer.
	 */
	class LimitedEndInputStream : public ReadOnlyInputStream
	{
	public:
		//Constructor
		inline LimitedEndInputStream(InputStream& baseInputStream, uint32 nEndBytes) : baseInputStream(baseInputStream), nEndBytes(nEndBytes)
		{
			ASSERT(nEndBytes < sizeof (this->buffer), u8"Report this please!");
			ASSERT_EQUALS(nEndBytes, baseInputStream.ReadBytes(buffer, nEndBytes));
		}

		//Properties
		inline const void* EndBytes() const
		{
			return this->buffer;
		}

		//Methods
		uint32 GetBytesAvailable() const override;
		bool IsAtEnd() const override;
		uint32 ReadBytes(void *destination, uint32 count) override;

	private:
		//Members
		InputStream& baseInputStream;
		/**
		 * Invariant: buffer always holds exactly 'nEndBytes'
		 */
		uint8 buffer[c_io_blockSize];
		const uint32 nEndBytes;

		//Inline
		inline void CopyBackIntoBuffer(uint8*& source, uint32 count)
		{
			uint8* dest = &this->buffer[count];
			while(count--)
				*--dest = *--source;
		}

		inline void FillFromBuffer(uint8*& destination, uint32& count)
		{
			uint32 nBytesToCopy = Math::Min(this->nEndBytes, count);

			MemCopy(destination, this->buffer, nBytesToCopy);

			destination += nBytesToCopy;
			count -= nBytesToCopy;
		}

		inline uint32 RefillBuffer()
		{
			return this->nEndBytes - this->baseInputStream.ReadBytes(this->buffer, this->nEndBytes);
		}

		inline void ShiftBuffer(uint8 shiftAmount)
		{
			for(int32 i = this->nEndBytes - 1; i >= 0; i--)
				this->buffer[i + shiftAmount] = this->buffer[i];
		}
	};
}
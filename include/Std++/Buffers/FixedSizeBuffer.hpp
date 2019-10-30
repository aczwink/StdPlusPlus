/*
* Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Unsigned.hpp>
#include <Std++/Streams/Writers/DataWriter.hpp>
#include <Std++/Streams/BufferOutputStream.hpp>

namespace StdXX
{
	class FixedSizeBuffer
	{
	public:
		//Constructor
		inline FixedSizeBuffer(uint32 size) : _size(size)
		{
			this->_data = (byte*)MemAlloc(size);
		}

		//Destructor
		inline ~FixedSizeBuffer()
		{
			MemFree(this->_data);
		}

		//Properties
		inline uint8* data()
		{
			return this->_data;
		}

		inline const uint8* data() const
		{
			return this->_data;
		}

		inline uint32 size() const
		{
			return this->_size;
		}

		//Methods
		uint32 FindBytesReversed(const byte* bytes, uint32 size, uint32 startOffset = StdXX::Unsigned<uint32>::Max());

		//Inline
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
		byte* _data;
		uint32 _size;
	};
}
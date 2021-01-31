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
#include "ReadOnlyInputStream.hpp"

namespace StdXX
{
	template<uint32 bufferSize>
	class BlockInputStream : public ReadOnlyInputStream
	{
	public:
		//Constructor
		inline BlockInputStream(InputStream& baseInputStream) : baseInputStream(baseInputStream)
		{
			this->nBytesInBuffer = 0;
		}

		//Methods
		uint32 GetBytesAvailable() const
		{
			return this->nBytesInBuffer + this->baseInputStream.GetBytesAvailable();
		}

		bool IsAtEnd() const
		{
			return (this->nBytesInBuffer == 0) && this->baseInputStream.IsAtEnd();
		}

		uint32 ReadBytes(void *destination, uint32 count)
		{
			uint8* dest = static_cast<uint8 *>(destination);
			while(count)
			{
				if(this->nBytesInBuffer == 0)
				{
					if(this->IsAtEnd())
						break;

					this->bufferOffset = 0;
					this->nBytesInBuffer = this->ReadNextBlock(this->baseInputStream, this->buffer);
				}

				uint32 nBytesToCopy = Math::Min(count, this->nBytesInBuffer);
				MemCopy(dest, &this->buffer[this->bufferOffset], nBytesToCopy);

				this->bufferOffset += nBytesToCopy;
				this->nBytesInBuffer -= nBytesToCopy;

				dest += nBytesToCopy;
				count -= nBytesToCopy;
			}

			return dest - static_cast<uint8 *>(destination);
		}

	private:
		//Members
		InputStream& baseInputStream;
		uint8 buffer[bufferSize];
		uint32 nBytesInBuffer;
		uint32 bufferOffset;

		//Abstract
		virtual uint32 ReadNextBlock(InputStream& baseInputStream, void* destination) = 0;
	};
}
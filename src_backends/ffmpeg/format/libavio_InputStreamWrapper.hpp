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
#pragma once
//Global
extern "C"
{
#include <libavformat/avformat.h>
}
//Local
#include <Std++/Streams/SeekableInputStream.hpp>

namespace _stdxx_
{
	class libavio_InputStreamWrapper
	{
	public:
		//Constructor
		libavio_InputStreamWrapper(StdXX::SeekableInputStream& seekableInputStream);

		//Destructor
		~libavio_InputStreamWrapper();

		//Properties
		inline AVIOContext* Context()
		{
			return this->avio_ctx;
		}

		//Inline
		inline uint64 QuerySize() const
		{
			return this->seekableInputStream.QuerySize();
		}

		inline uint32 ReadPacket(void* destination, uint32 size)
		{
			return this->seekableInputStream.ReadBytes(destination, size);
		}

		inline uint64 SeekTo(uint64 offset)
		{
			this->seekableInputStream.SeekTo(offset);
			return this->seekableInputStream.QueryCurrentOffset();
		}

	private:
		//Members
		StdXX::SeekableInputStream& seekableInputStream;
		uint8* avio_ctx_buffer;
		AVIOContext *avio_ctx;
	};
}
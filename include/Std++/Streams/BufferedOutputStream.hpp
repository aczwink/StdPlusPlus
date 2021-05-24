/*
 * Copyright (c) 2019,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Constants.hpp>
#include "OutputStream.hpp"

namespace StdXX
{
	class BufferedOutputStream : public OutputStream
	{
	public:
		//Constructor
		BufferedOutputStream(OutputStream& outputStream, uint32 bufferSize = c_io_blockSize);

		//Destructor
		~BufferedOutputStream();

		//Methods
		void Flush() override;
		uint32 WriteBytes(const void *source, uint32 size) override;

	private:
		//Members
		OutputStream& outputStream;
		byte* buffer;
		uint32 offset;
		const uint32 bufferSize;

		//Inline
		inline void WriteBufferToBaseStream()
		{
			this->outputStream.WriteBytes(this->buffer, this->offset);
			this->offset = 0;
		}
	};
}
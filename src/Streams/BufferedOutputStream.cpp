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
//Class header
#include <Std++/Streams/BufferedOutputStream.hpp>
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;

//Constructor
BufferedOutputStream::BufferedOutputStream(OutputStream &outputStream, uint32 bufferSize) : outputStream(outputStream), offset(0), bufferSize(bufferSize)
{
	this->buffer = static_cast<byte *>(MemAlloc(bufferSize));
}

//Destructor
BufferedOutputStream::~BufferedOutputStream()
{
	MemFree(this->buffer);
}

//Public methods
void BufferedOutputStream::Flush()
{
	this->outputStream.WriteBytes(this->buffer, this->offset);
	this->offset = 0;
}

uint32 BufferedOutputStream::WriteBytes(const void *source, uint32 size)
{
	uint32 bytesWritten = 0;

	const byte* src = static_cast<const byte *>(source);
	while(size)
	{
		uint32 bytesToWrite = Math::Min(size, this->bufferSize - this->offset);
		MemCopy(this->buffer + this->offset, src, bytesToWrite);

		src += bytesToWrite;
		size -= bytesToWrite;
		bytesWritten += bytesToWrite;
		this->offset += bytesToWrite;

		if(this->offset == this->bufferSize)
			this->Flush();
	}

	return bytesWritten;
}

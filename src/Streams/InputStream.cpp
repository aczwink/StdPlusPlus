/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/InputStream.hpp>
//Local
#include <Std++/Streams/OutputStream.hpp>
//Namespaces
using namespace StdXX;

//Public methods
uint32 InputStream::FlushTo(OutputStream &refOutput, uint32 size)
{
	uint32 nReadBytes, nTotalReadBytes, leftSize;
	byte buffer[4096];

	nTotalReadBytes = 0;
	while(size)
	{
		leftSize = size;
		if(leftSize > sizeof(buffer))
			leftSize = sizeof(buffer);

		nReadBytes = this->ReadBytes(buffer, leftSize);
		if(!nReadBytes)
			break;
		refOutput.WriteBytes(buffer, nReadBytes);

		size -= nReadBytes;
		nTotalReadBytes += nReadBytes;
	}

	return nTotalReadBytes;
}
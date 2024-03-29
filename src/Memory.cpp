/*
 * Copyright (c) 2017-2021 Amir Czwink (amir130@hotmail.de)
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
//corresponding header
#include <Std++/Memory.hpp>
//Local
#include <Std++/Mathematics.hpp>

//Namespace functions
void StdXX::MemMove(void *destination, const void *source, uint32 size)
{
	const uint8* src = static_cast<const uint8 *>(source);
	uint8* dest = static_cast<uint8 *>(destination);

	if(Math::IsValueInInterval((const uint8*)dest, src, src+size))
	{
		//overlap, copy from end
		src += size - 1;
		dest += size - 1;
		while(size--)
		{
			*dest-- = *src--;
		}
		return;
	}

	MemCopy(destination, source, size);
}
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
#include <Std++/Streams/ChainedInputStream.hpp>
//Namespaces
using namespace StdXX;

//Public methods
uint32 ChainedInputStream::GetBytesAvailable() const
{
	return this->end->GetBytesAvailable();
}

bool ChainedInputStream::IsAtEnd() const
{
	return this->end->IsAtEnd();
}

uint32 ChainedInputStream::ReadBytes(void *destination, uint32 count)
{
	return this->end->ReadBytes(destination, count);
}

uint32 ChainedInputStream::Skip(uint32 nBytes)
{
	return this->end->Skip(nBytes);
}

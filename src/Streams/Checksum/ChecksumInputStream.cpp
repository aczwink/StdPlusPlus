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
//Class header
#include <Std++/Streams/Checksum/ChecksumInputStream.hpp>
//Namespaces
using namespace StdXX;

//Public methods
uint32 ChecksumInputStream::GetBytesAvailable() const
{
	return this->baseInputStream.GetBytesAvailable();
}

bool ChecksumInputStream::IsAtEnd() const
{
	return this->baseInputStream.IsAtEnd();
}

uint32 ChecksumInputStream::ReadBytes(void *destination, uint32 count)
{
	uint32 nBytesRead = this->baseInputStream.ReadBytes(destination, count);
	this->checkFunc->Update(destination, nBytesRead);

	return nBytesRead;
}

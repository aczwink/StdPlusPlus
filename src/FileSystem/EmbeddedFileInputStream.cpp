/*
 * Copyright (c) 2018-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/FileSystem/Buffered/EmbeddedFileInputStream.hpp>
//Local
#include <Std++/Debug.hpp>
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;

//Constructor
EmbeddedFileInputStream::EmbeddedFileInputStream(uint64 offset, uint64 size, SeekableInputStream& baseInputStream, Mutex& baseInputStreamLock)
	: offset(offset), size(size), baseInputStream(baseInputStream), baseInputStreamLock(baseInputStreamLock), currentOffset(0)
{
}

//Public methods
uint32 EmbeddedFileInputStream::GetBytesAvailable() const
{
	return baseInputStream.GetBytesAvailable();
}

uint64 EmbeddedFileInputStream::QueryCurrentOffset() const
{
	return this->currentOffset;
}

uint64 EmbeddedFileInputStream::QueryRemainingBytes() const
{
	return this->QuerySize() - this->currentOffset;
}

uint64 EmbeddedFileInputStream::QuerySize() const
{
	return this->size;
}

bool EmbeddedFileInputStream::IsAtEnd() const
{
	return this->QueryRemainingBytes() == 0;
}

uint32 EmbeddedFileInputStream::ReadBytes(void *destination, uint32 count)
{
	count = Math::Min(count, static_cast<const uint32 &>(this->QueryRemainingBytes()));

	this->baseInputStreamLock.Lock();
	this->baseInputStream.SeekTo(this->offset + this->currentOffset);
	count = this->baseInputStream.ReadBytes(destination, count);
	this->baseInputStreamLock.Unlock();

	this->currentOffset += count;

	return count;
}

void EmbeddedFileInputStream::SeekTo(uint64 offset)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

uint32 EmbeddedFileInputStream::Skip(uint32 nBytes)
{
	nBytes = Math::Min(nBytes, static_cast<uint32>(this->QueryRemainingBytes()));
	this->currentOffset += nBytes;
	return nBytes;
}
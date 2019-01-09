/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Filesystem/ContainerFileInputStream.hpp>
//Local
#include <Std++/Debug.hpp>
#include <Std++/Filesystem/ContainerFile.hpp>
#include <Std++/Filesystem/ContainerFileSystem.hpp>
//Namespaces
using namespace StdXX;

//Constructor
ContainerFileInputStream::ContainerFileInputStream(const ContainerFile &file) : file(file)
{
	this->currentOffset = file.header.offset;
}

//Public methods
uint64 ContainerFileInputStream::GetCurrentOffset() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

uint64 ContainerFileInputStream::GetRemainingBytes() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

uint64 ContainerFileInputStream::GetSize() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

bool ContainerFileInputStream::IsAtEnd() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return false;
}

uint32 ContainerFileInputStream::ReadBytes(void *destination, uint32 count)
{
	//validate that count is in range
	uint64 endOffset;
	if(this->file.buffer.IsNull())
		endOffset = this->file.header.offset + this->file.header.compressedSize;
	else
		endOffset = this->file.buffer->GetNumberOfElements();

	if (this->currentOffset < endOffset)
	{
		uint64 maxBytes = endOffset - this->currentOffset;
		if (count > maxBytes)
			count = static_cast<uint32>(maxBytes);
	}
	else
		count = 0;

	//read
	if(this->file.buffer.IsNull())
	{
		ContainerFileSystem *fs = (ContainerFileSystem *) this->file.GetFileSystem();
		fs->containerInputStream->SetCurrentOffset(this->currentOffset);
		fs->containerInputStream->ReadBytes(destination, count);
	}
	else
		this->file.buffer->PeekBytes(destination, this->currentOffset, count);

	this->currentOffset += count;
	return count;
}

void ContainerFileInputStream::SetCurrentOffset(uint64 offset)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

uint32 ContainerFileInputStream::Skip(uint32 nBytes)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

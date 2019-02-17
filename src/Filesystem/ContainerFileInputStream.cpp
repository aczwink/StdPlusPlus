/*
 * Copyright (c) 2018-2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;

//Constructor
ContainerFileInputStream::ContainerFileInputStream(const ContainerFile &file) : file(file), currentOffset(0)
{
}

//Public methods
uint64 ContainerFileInputStream::GetCurrentOffset() const
{
	return this->currentOffset;
}

uint64 ContainerFileInputStream::GetRemainingBytes() const
{
	return this->GetSize() - this->currentOffset;
}

uint64 ContainerFileInputStream::GetSize() const
{
	return this->file.GetHeader().compressedSize; //we only read the data block
}

bool ContainerFileInputStream::IsAtEnd() const
{
	return this->GetRemainingBytes() == 0;
}

uint32 ContainerFileInputStream::ReadBytes(void *destination, uint32 count)
{
	count = Math::Min(count, static_cast<const uint32 &>(this->GetRemainingBytes()));

	ContainerFileSystem *fs = (ContainerFileSystem *) this->file.GetFileSystem();
	fs->containerInputStream->SetCurrentOffset(this->file.GetHeader().offset + this->currentOffset);
	count = fs->containerInputStream->ReadBytes(destination, count);

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
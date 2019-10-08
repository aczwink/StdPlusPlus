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
#include <Std++/Filesystem/ContainerFile.hpp>
//Local
#include <Std++/Compression/Decompressor.hpp>
#include <Std++/Filesystem/ContainerDirectory.hpp>
#include <Std++/Filesystem/ContainerFileInputStream.hpp>
#include <Std++/Streams/ChainedInputStream.hpp>
#include <Std++/Streams/BufferedInputStream.hpp>
//Namespaces
using namespace StdXX;

//Public methods
uint64 ContainerFile::GetSize() const
{
	return this->header.uncompressedSize;
}

UniquePointer<InputStream> ContainerFile::OpenForReading(bool verify) const
{
	InputStream* input = new ContainerFileInputStream(*this);
	if (this->header.compression.HasValue())
	{
		ChainedInputStream* chain = new ChainedInputStream(input);
		chain->Add(new BufferedInputStream(chain->GetEnd())); //add a buffer for performance
		chain->Add(Decompressor::Create(*this->header.compression, chain->GetEnd(), verify));
		return chain;
	}
	return input;
}

UniquePointer<OutputStream> ContainerFile::OpenForWriting()
{
	//can not open read only file for writing
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

FileSystemNodeInfo ContainerFile::QueryInfo() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return FileSystemNodeInfo();
}

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
#include <Std++/Filesystem/ContainerFile.hpp>
//Local
#include <Std++/Compression/Decompressor.hpp>
#include <Std++/Filesystem/ContainerDirectory.hpp>
#include <Std++/Filesystem/ContainerFileInputStream.hpp>
//Namespaces
using namespace StdXX;

//Public methods
FileSystem *ContainerFile::GetFileSystem()
{
	return this->parent->GetFileSystem();
}

const FileSystem *ContainerFile::GetFileSystem() const
{
	return this->parent->GetFileSystem();
}

String ContainerFile::GetName() const
{
	return this->name;
}

AutoPointer<Directory> ContainerFile::GetParent() const
{
	return (Directory *)this->parent;
}

Path ContainerFile::GetPath() const
{
	return this->parent->GetPath() / this->name;
}

uint64 ContainerFile::GetSize() const
{
	if(!this->buffer.IsNull())
	{
		//file has been overwritten and not yet flushed
		return this->buffer->GetSize();
	}

	return this->header.uncompressedSize;
}

UniquePointer<InputStream> ContainerFile::OpenForReading() const
{
	InputStream* input = new ContainerFileInputStream(*this);
	if (this->header.compression.HasValue())
	{
		NOT_IMPLEMENTED_ERROR; //TODO: this currently will lack the above InputStream "input". When we return the decompressor, we need to free the above instance at the time the destructor of the decompressor is called
		return Decompressor::Create(*this->header.compression, *input);
	}
	return input;
}

UniquePointer<OutputStream> ContainerFile::OpenForWriting()
{
	if(this->buffer.IsNull())
	{
		this->buffer = new FIFOBuffer;
		this->buffer->SetAllocationInterval(0);
	}

	class ContainerFileOutputStream : public OutputStream
	{
	public:
		//Constructor
		ContainerFileOutputStream(UniquePointer<FIFOBuffer> &fileBuffer) : fileBuffer(fileBuffer)
		{
		}

		//Methods
		void Flush() override
		{
			this->fileBuffer->Flush();
		}

		uint32 WriteBytes(const void *source, uint32 size) override
		{
			return this->fileBuffer->WriteBytes(source, size);
		}

	private:
		//Members
		UniquePointer<FIFOBuffer> &fileBuffer;
	};

	return new ContainerFileOutputStream(this->buffer);
}

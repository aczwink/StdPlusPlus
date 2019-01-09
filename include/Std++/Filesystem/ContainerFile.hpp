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
#pragma once
//Local
#include <Std++/Compression/CompressionAlgorithm.hpp>
#include "Std++/Containers/FIFOBuffer.hpp"
#include <Std++/Optional.hpp>
#include "../Definitions.h"
#include "File.hpp"

namespace StdXX
{
	//Forward declarations
	class ContainerDirectory;

	struct ContainerFileHeader
	{
		Optional<CompressionAlgorithm> compression;
		uint64 offset;
		uint64 uncompressedSize;
		uint64 compressedSize;

		inline ContainerFileHeader() : offset(0), uncompressedSize(0), compressedSize(0)
		{
		}
	};

	class ContainerFile : public File
	{
		friend class ContainerDirectory;
		friend class ContainerFileInputStream;
	public:
		//Constructor
		inline ContainerFile(const String &name, ContainerDirectory *parent) : name(name), parent(parent)
		{
		}

		//Methods
		FileSystem *GetFileSystem() override;
		const FileSystem *GetFileSystem() const override;
		String GetName() const override;
		AutoPointer<Directory> GetParent() const override;
		Path GetPath() const override;
		uint64 GetSize() const override;
		UniquePointer<InputStream> OpenForReading() const override;
		UniquePointer<OutputStream> OpenForWriting() override;

	private:
		//Members
		String name;
		ContainerDirectory *parent;
		ContainerFileHeader header;
		/**
		 * Data written to the file but not yet flushed to the output container.
		 */
		UniquePointer<FIFOBuffer> buffer;
	};
}
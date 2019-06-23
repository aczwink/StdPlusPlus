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
#pragma once
//Local
#include <Std++/Compression/CompressionAlgorithm.hpp>
#include <Std++/Optional.hpp>
#include "../Definitions.h"
#include "File.hpp"

namespace StdXX
{
	//Forward declarations
	class ContainerFileSystem;

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
	public:
		//Constructor
		inline ContainerFile(const ContainerFileHeader& header, ContainerFileSystem* fileSystem) : header(header), fileSystem(fileSystem)
		{
		}

		//Methods
		uint64 GetSize() const override;
		uint64 GetStoredSize() const override;
		UniquePointer<InputStream> OpenForReading(bool verify) const override;
		UniquePointer<OutputStream> OpenForWriting() override;

		//Inline
		inline const ContainerFileHeader& GetHeader() const
		{
			return this->header;
		}

		inline const ContainerFileSystem* GetFileSystem() const
		{
			return this->fileSystem;
		}

	private:
		//Members
		ContainerFileHeader header;
		ContainerFileSystem* fileSystem;
	};
}
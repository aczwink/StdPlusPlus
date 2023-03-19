/*
 * Copyright (c) 2018-2023 Amir Czwink (amir130@hotmail.de)
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
#include "FileMetadataNode.hpp"

namespace StdXX::FileSystem
{
	struct ContainerFileHeader : public FileInfo
	{
		Optional<CompressionStreamFormatType> compression;
		uint64 offset;

		inline ContainerFileHeader() : offset(0)
		{
			this->size = 0;
			this->storedSize = 0;
		}
	};

	class ContainerFileBase : public FileMetadataNode
	{
	public:
		const ContainerFileHeader &Info() const override = 0;
	};

	template<typename T>
	class ContainerFile : public ContainerFileBase
	{
	public:
		//Constructor
		inline ContainerFile(const T& header) : header(header)
		{
			this->CheckCompressedSize();
		}

		inline ContainerFile(T&& header) : header(Move(header))
		{
			this->CheckCompressedSize();
		}

		const ContainerFileHeader &Info() const override
		{
			return this->header;
		}

	private:
		//Members
		T header;

		//Inline
		inline void CheckCompressedSize()
		{
			if(!this->header.compression.HasValue() && (this->header.storedSize == 0))
				this->header.storedSize = header.size;
		}
	};
}
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
#pragma once
//Local
#include <Std++/Compression/CompressionAlgorithm.hpp>
#include <Std++/Optional.hpp>
#include "Std++/Definitions.h"
#include "File.hpp"

namespace StdXX::FileSystem
{
	//Forward declarations
	class ArchiveFileSystem;

	class ContainerFile : public File
	{
	public:
		//Methods
		void ChangePermissions(const FileSystem::NodePermissions &newPermissions) override;
		UniquePointer<InputStream> OpenForReading(bool verify) const override;
		UniquePointer<OutputStream> OpenForWriting() override;
		NodeInfo QueryInfo() const override;

	private:
		//Members
		ContainerFileHeader header;
		ArchiveFileSystem* fileSystem;
		UniquePointer<NodePermissions> permissions;
	};
}
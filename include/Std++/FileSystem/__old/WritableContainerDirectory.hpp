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
#include "Std++/Containers/Map/Map.hpp"
#include "ContainerFile.hpp"
#include "MemoryDirectory.hpp"

namespace StdXX::FileSystem
{
	//Forward declarations
	class WritableContainerFileSystem;

	class WritableContainerDirectory : public MemoryDirectory
	{
		friend class ArchiveFileSystem;
	public:
		//Constructors
		inline WritableContainerDirectory(WritableContainerFileSystem *fileSystem) : fileSystem(fileSystem)
		{
		}

		//Methods
		UniquePointer<OutputStream> CreateFile(const String &name) override;
		void CreateSubDirectory(const String &name, const NodePermissions* permissions) override;

	private:
		//Members
		WritableContainerFileSystem *fileSystem;
	};
}
/*
 * Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
//Local
#include "../Containers/Map/Map.hpp"
#include "ContainerFile.hpp"
#include "MemoryDirectory.hpp"

namespace StdXX::FileSystem
{
	//Forward declarations
	class ContainerFileSystem;

	class ContainerDirectory : public MemoryDirectory
	{
		friend class ContainerFileSystem;
	public:
		//Constructors
		inline ContainerDirectory(ContainerFileSystem *fileSystem) : fileSystem(fileSystem), parent(nullptr)
		{
			//this is the root dir
		}

		inline ContainerDirectory(const String &name, ContainerDirectory *parent) :
			fileSystem(parent->fileSystem), name(name), parent(parent)
		{
		}

		//Methods
		void ChangePermissions(const FileSystem::NodePermissions &newPermissions) override;
		UniquePointer<OutputStream> CreateFile(const String &name) override;
		void CreateSubDirectory(const String &name) override;
		NodeInfo QueryInfo() const override;

	private:
		//Members
		ContainerFileSystem *fileSystem;
		String name;
		ContainerDirectory *parent;

		//Inline
		inline Path GetPath() const
		{
			if(this->parent)
				return this->parent->GetPath() / this->name;

			return Path(u8"/");
		}
	};
}
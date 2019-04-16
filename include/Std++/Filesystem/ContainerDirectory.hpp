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
//Local
#include "../Containers/Map/Map.hpp"
#include "Directory.hpp"
#include "ContainerFile.hpp"

namespace StdXX
{
	//Move declarations
	class ContainerFileSystem;

	class ContainerDirectory : public Directory
	{
		friend class ContainerFileSystem;
	public:
		//Constructors
		inline ContainerDirectory(ContainerFileSystem *fileSystem) : fileSystem(fileSystem), parent(nullptr)
		{
			//this is the root dir
		}

		inline ContainerDirectory(const String &name, ContainerDirectory *parent) :
			fileSystem(reinterpret_cast<ContainerFileSystem *>(parent->GetFileSystem())), name(name), parent(parent)
		{
		}

		//Methods
		bool ContainsFile(const String &name) const override;
		bool ContainsSubDirectory(const String &name) const override;
		UniquePointer<OutputStream> CreateFile(const String &name) override;
		void CreateSubDirectory(const String &name) override;
		bool Exists(const Path &path) const override;
		AutoPointer<const File> GetFile(const String &name) const override;
		FileSystem *GetFileSystem() override;
		const FileSystem *GetFileSystem() const override;
		AutoPointer<const Directory> GetParent() const override;
		Path GetPath() const override;
		uint64 GetSize() const override;
		uint64 GetStoredSize() const override;
		AutoPointer<Directory> GetSubDirectory(const String &name) override;
		AutoPointer<const Directory> GetSubDirectory(const String &name) const override;

		//For range-based loop
		DirectoryIterator begin() const override;
		DirectoryIterator end() const override;

	private:
		//Members
		ContainerFileSystem *fileSystem;
		String name;
		ContainerDirectory *parent;
		Map<String, AutoPointer<File>> files;
		Map<String, AutoPointer<ContainerDirectory>> subDirectories;

		//Methods
		void AddSourceFile(String fileName, const ContainerFileHeader& header);
	};
}
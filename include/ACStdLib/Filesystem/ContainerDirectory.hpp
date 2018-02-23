/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Local
#include "../Containers/Map/Map.hpp"
#include "Directory.hpp"
#include "ContainerFile.hpp"

namespace ACStdLib
{
	class ContainerDirectory : public Directory
	{
		friend class ContainerFileSystem;
	public:
		//Methods
		bool ContainsSubDirectory(const String &name) const override;
		void CreateSubDirectory(const String &name) override;
		uint64 GetSize() const override;
		AutoPointer<Directory> GetSubDirectory(const String &name) override;




		bool ContainsFile(const String &name) const override
		{
			return false;
		}

		FileSystem *GetFileSystem() override
		{
			NOT_IMPLEMENTED_ERROR;
			return nullptr;
		}

		//For range-based loop
		DirectoryIterator begin() const;
		DirectoryIterator end() const;

	private:
		//Members
		Map<String, AutoPointer<ContainerFile>> files;
		Map<String, AutoPointer<ContainerDirectory>> subDirectories;

		//Methods
		void AddSourceFile(String fileName, uint64 offset, uint64 size);
	};
}
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
#include "Std++/Containers/FIFOBuffer.hpp"
#include "../Definitions.h"
#include "File.hpp"

namespace StdPlusPlus
{
	//Move declarations
	class ContainerDirectory;

	class ContainerFile : public File
	{
		friend class ContainerDirectory;
		friend class ContainerFileInputStream;
	public:
		//Constructor
		inline ContainerFile(const String &name, ContainerDirectory *parent) : name(name), parent(parent), offset(0), size(0)
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
		uint64 offset;
		uint64 size;
		/**
		 * Data written to the file but not yet flushed to the output container.
		 */
		UniquePointer<FIFOBuffer> buffer;
	};
}
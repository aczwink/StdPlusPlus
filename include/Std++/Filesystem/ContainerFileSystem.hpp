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
#include <Std++/Streams/FileInputStream.hpp>
#include <Std++/Streams/FileOutputStream.hpp>
#include "../Streams/SeekableInputStream.hpp"
#include "FileSystem.hpp"

namespace StdXX
{
	class ContainerFileSystem : public FileSystem
	{
		friend class ContainerDirectory;
		friend class ContainerFileInputStream;
	public:
		//Constructor
		ContainerFileSystem(const FileSystemFormat *format, const Path &fileSystemPath);

		//Methods
		UniquePointer<OutputStream> CreateFile(const Path &filePath) override;
		bool Exists(const Path &path) const override;
		AutoPointer<Directory> GetDirectory(const Path &directoryPath) override;
		AutoPointer<Directory> GetRoot() override;
		uint64 GetSize() const override;
		void Move(const Path &from, const Path &to) override;

	protected:
		//Members
		AutoPointer<Directory> root;
		Path fileSystemPath;
		UniquePointer<FileInputStream> containerInputStream;
		bool isFlushed;

		//Methods
		void AddSourceFile(const Path &path, uint64 offset, uint64 size);
		UniquePointer<FileOutputStream> OpenTempContainer();
		void SwapWithTempContainer(UniquePointer<FileOutputStream> &tempContainer);
	};
}
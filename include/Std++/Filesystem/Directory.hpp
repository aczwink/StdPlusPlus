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
#include "../SmartPointers/AutoPointer.hpp"
#include "../SmartPointers/UniquePointer.hpp"
#include "../Streams/OutputStream.hpp"
#include "FileSystemNode.hpp"
#include "Path.hpp"

namespace StdXX
{
	//Move declarations
	class DirectoryWalkerWrapper;

	class Directory : public FileSystemNode
	{
	public:
		//Destructor
		virtual ~Directory()
		{
		}

		//Abstract
		virtual bool ContainsFile(const String &name) const = 0;
		virtual bool ContainsSubDirectory(const String &name) const = 0;
		virtual UniquePointer<OutputStream> CreateFile(const String &name) = 0;
		virtual void CreateSubDirectory(const String &name) = 0;
		virtual bool Exists(const Path &path) const = 0;
		virtual AutoPointer<Directory> GetSubDirectory(const String &name) = 0;
		virtual AutoPointer<const Directory> GetSubDirectory(const String &name) const = 0;

		//Methods
		void CreateDirectoryTree(const Path &directoryPath);
		DirectoryWalkerWrapper WalkFiles();

		//For range-based loop
		virtual DirectoryIterator begin() const = 0;
		virtual DirectoryIterator end() const = 0;
	};
}
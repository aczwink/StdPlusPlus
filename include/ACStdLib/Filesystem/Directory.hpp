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
#pragma once
//Local
#include "../SmartPointers/AutoPointer.hpp"
#include "FileSystemNode.hpp"
#include "Path.hpp"

namespace ACStdLib
{
	//Forward declarations
	class FileSystem;

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
		virtual void CreateSubDirectory(const String &name) = 0;
		virtual FileSystem *GetFileSystem() = 0;
		virtual AutoPointer<Directory> GetSubDirectory(const String &name) = 0;

		//Methods
		void CreateDirectoryTree(const Path &directoryPath);
	};
}
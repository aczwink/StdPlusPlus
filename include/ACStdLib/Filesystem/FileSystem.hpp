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
#include "../SmartPointers/UniquePointer.hpp"
#include "Directory.hpp"

namespace ACStdLib
{
	//Forward declarations
	class FileSystemFormat;

	class FileSystem
	{
	public:
		//Constructor
		inline FileSystem(const FileSystemFormat *format) : fileSystemFormat(format)
		{
		}

		//Destructor
		virtual ~FileSystem()
		{
		}

		//Abstract
		//virtual bool Exists(const Path &path) const = 0; //TODO
		virtual AutoPointer<Directory> GetDirectory(const Path &directoryPath) = 0;
		virtual AutoPointer<Directory> GetRoot() = 0;
		virtual uint64 GetSize() const = 0;

		//Methods
		void CreateDirectoryTree(const Path &directoryPath);

		//Functions
		static UniquePointer<FileSystem> LoadFromFile(const Path &p);

		//Inline
		inline const FileSystemFormat *GetFormat() const
		{
			return this->fileSystemFormat;
		}

	private:
		//Members
		const FileSystemFormat *fileSystemFormat;
	};
}
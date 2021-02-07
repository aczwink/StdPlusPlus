/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/SmartPointers/UniquePointer.hpp>
#include "Path.hpp"
#include "Permissions.hpp"

namespace StdXX::FileSystem
{
	class WritableFileSystem
	{
	public:
		//Destructor
		virtual ~WritableFileSystem() = default;

		//Abstract
		/**
		 *
		 * @param name
		 * @param permissions - pass null for default permissions
		 */
		virtual void CreateDirectory(const Path& path, const Permissions* permissions = nullptr) = 0;
		virtual UniquePointer<OutputStream> CreateFile(const Path &filePath) = 0;
		virtual void CreateLink(const Path& linkPath, const Path& linkTargetPath) = 0;
		virtual void DeleteFile(const Path& path) = 0;
		virtual void Flush() = 0;
		virtual void Move(const Path &from, const Path &to) = 0;
		/*
		 * The following special cases aren't implemented yet. They don't need a convenience class like FileOutputStream.
		 * OpenForAppending:
		 * -Create file in case it does not exist
		 * -File pos is at end
		 * -File is of course not truncated
		 *
		 * OpenForUpdating: no mode needed because this would do the following:
		 * -Create file in case it does not exist
		 * -File pos is at beginning
		 * -File is of course not truncated
		 * -File must support streaming
		 *
		 */
		/**
		 * File is truncated.
		 *
		 * @return
		 */
		virtual UniquePointer<OutputStream> OpenFileForWriting(const Path& path) = 0;
		virtual void RemoveDirectory(const Path& path) = 0;
	};
}
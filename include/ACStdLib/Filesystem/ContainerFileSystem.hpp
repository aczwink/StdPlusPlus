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
#include "../Streams/SeekableInputStream.hpp"
#include "FileSystem.hpp"

namespace ACStdLib
{
	class ContainerFileSystem : public FileSystem
	{
	public:
		//Constructor
		ContainerFileSystem(const FileSystemFormat *format, UniquePointer<SeekableInputStream> &&inputStream);

		//Methods
		AutoPointer<Directory> GetDirectory(const Path &directoryPath) override;
		AutoPointer<Directory> GetRoot() override;
		uint64 GetSize() const override;

	protected:
		//Members
		AutoPointer<Directory> root;
		UniquePointer<SeekableInputStream> inputStream;

		//Methods
		void AddSourceFile(const Path &path, uint64 offset, uint64 size);
	};
}
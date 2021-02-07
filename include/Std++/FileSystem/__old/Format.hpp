/*
 * Copyright (c) 2018-2021 Amir Czwink (amir130@hotmail.de)
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
#include "Std++/Streams/SeekableInputStream.hpp"
#include "RWFileSystem.hpp"

namespace StdXX::FileSystem
{
	struct OpenOptions
	{
		Optional<String> password;
	};

	class Format
	{
	public:
		//Abstract
		virtual RWFileSystem* CreateFileSystem(const Path &fileSystemPath) const = 0;
		virtual String GetId() const = 0;
		virtual String GetName() const = 0;
		virtual float32 Matches(SeekableInputStream &inputStream) const = 0;
		virtual RWFileSystem *OpenFileSystem(const Path &fileSystemPath, const OpenOptions& options) const = 0;
		virtual ReadableFileSystem *OpenFileSystemReadOnly(const Path &fileSystemPath, const OpenOptions& options) const = 0;

		//Functions
		static const Format* FindBestFormat(const Path& path);
		static const Format* GetFormatById(const String& id);
		static STDPLUSPLUS_API void Register(const Format *fileSystemFormat);
	};
}
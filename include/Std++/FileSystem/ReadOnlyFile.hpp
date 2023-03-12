/*
 * Copyright (c) 2018-2023 Amir Czwink (amir130@hotmail.de)
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
#include "DirectoryWalker.hpp"

namespace StdXX::FileSystem
{
	class ReadOnlyFile
	{
	public:
		//Constructor
		inline ReadOnlyFile(const ReadableFileSystem& fileSystem, const Path& path) : path(path), fileSystem(fileSystem)
		{
			this->hasFileInfoCached = false;
		}

		//Properties
		inline bool Exists() const
		{
			return this->CachedFileInfo().HasValue();
		}

		inline const FileInfo& Info() const
		{
			return *this->CachedFileInfo();
		}

		inline const class Path& Path() const
		{
			return this->path;
		}

		inline uint64 Size() const
		{
			return this->CachedFileInfo()->size;
		}

		inline FileType Type() const
		{
			return this->CachedFileInfo()->type;
		}

		//Inline
		inline ReadOnlyFile Child(const String& childName) const
		{
			return {this->fileSystem, this->path / childName};
		}

		inline void InvalidateCache()
		{
			this->hasFileInfoCached = false;
		}

		inline bool IsEmptyDirectory() const
		{
			for(const DirectoryEntry& child : *this)
				return false;
			return true;
		}

		inline UniquePointer<InputStream> OpenForReading(bool verify = true) const
		{
			return this->fileSystem.OpenFileForReading(this->path, verify);
		}

		inline StdXX::FileSystem::Path ReadLinkTarget() const
		{
			auto value = this->fileSystem.ReadLinkTarget(this->path);
			ASSERT(value.HasValue(), u8"Can't read link target from non-link");
			return value.Value();
		}

		inline DirectoryWalkerWrapper WalkFiles()
		{
			return DirectoryWalkerWrapper(this->fileSystem, this->path);
		}

		//For range-based loop
		EnumeratorIterator<DirectoryEnumerator, DirectoryEntry> begin() const
		{
			return {new SelfAndParentFiltereredDirectoryEnumerator(this->fileSystem.EnumerateChildren(this->path))};
		}

		EnumeratorIterator<DirectoryEnumerator, DirectoryEntry> end() const
		{
			return {};
		}

	protected:
		//Members
		class Path path;

	private:
		//Members
		const ReadableFileSystem& fileSystem;
		mutable bool hasFileInfoCached;
		mutable Optional<FileInfo> fileInfo;

		//Inline
		inline Optional<FileInfo>& CachedFileInfo() const
		{
			if(!this->hasFileInfoCached)
			{
				this->fileInfo = Move(this->fileSystem.QueryFileInfo(this->path));
				this->hasFileInfoCached = true;
			}

			return this->fileInfo;
		}
	};
}
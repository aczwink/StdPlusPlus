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

		inline uint64 Size() const
		{
			return this->CachedFileInfo()->size;
		}

		inline FileType Type() const
		{
			return this->CachedFileInfo()->type;
		}

		//Inline
		inline UniquePointer<InputStream> OpenForReading(bool verify = true) const
		{
			return this->fileSystem.OpenFileForReading(this->path, verify);
		}

		//For range-based loop
		DirectoryIterator begin() const
		{
			return DirectoryIterator(this->fileSystem.EnumerateChildren(this->path));
		}

		DirectoryIterator end() const
		{
			return DirectoryIterator();
		}

	protected:
		//Members
		Path path;

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
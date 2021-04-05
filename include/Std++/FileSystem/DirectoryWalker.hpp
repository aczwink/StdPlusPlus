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
#include <Std++/Containers/LinkedList/LinkedList.hpp>
#include <Std++/FileSystem/Path.hpp>
#include "ReadableFileSystem.hpp"
#include <Std++/EnumeratorIterator.hpp>

namespace StdXX::FileSystem
{
	class DirectoryWalker
	{
		struct WalkerState
		{
			UniquePointer<DirectoryEnumerator> enumerator;
			Path path;

			inline WalkerState(UniquePointer<DirectoryEnumerator>&& enumerator, const Path& path) : enumerator(Move(enumerator)), path(path)
			{
			}

			WalkerState(WalkerState&&) = default;
			WalkerState &operator=(WalkerState&&) = default;

			WalkerState(const WalkerState&) = delete;
			WalkerState &operator=(const WalkerState&) = delete;
		};
	public:
		//Constructors
		inline DirectoryWalker(const ReadableFileSystem& fileSystem) : fileSystem(fileSystem)
		{
		}

		inline DirectoryWalker(const ReadableFileSystem& fileSystem, const Path& path) : fileSystem(fileSystem)
		{
			this->AddState(path);
		}

		//Methods
		bool Next(Path& entry);

	private:
		//Members
		const ReadableFileSystem& fileSystem;
		LinkedList<WalkerState> states;

		//Inline
		inline void AddState(const Path& path)
		{
			UniquePointer<DirectoryEnumerator> enumerator = new SelfAndParentFiltereredDirectoryEnumerator(this->fileSystem.EnumerateChildren(path));
			this->states.InsertTail(WalkerState(Move(enumerator), path));
		}
	};

	class DirectoryWalkerWrapper
	{
	public:
		//Constructor
		inline DirectoryWalkerWrapper(const ReadableFileSystem& fileSystem, const Path& path) : fileSystem(fileSystem), path(path)
		{
		}

		//For range-based loop
		inline EnumeratorIterator<DirectoryWalker, Path> begin() const
		{
			return UniquePointer<DirectoryWalker>(new DirectoryWalker(this->fileSystem, this->path));
		}

		inline EnumeratorIterator<DirectoryWalker, Path> end() const
		{
			return {};
		}

	private:
		//Members
		const ReadableFileSystem& fileSystem;
		Path path;
	};
}
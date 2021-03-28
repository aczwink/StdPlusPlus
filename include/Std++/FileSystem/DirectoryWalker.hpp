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
#include "DirectoryIterator.hpp"

namespace StdXX::FileSystem
{
	class DirectoryWalker
	{
		struct WalkerState
		{
			UniquePointer<DirectoryIterator> iterator;
			Path path;

			inline WalkerState(UniquePointer<DirectoryIterator>&& iterator, const Path& path) : iterator(Move(iterator)), path(path)
			{
			}

			inline WalkerState(WalkerState &&rhs)
			{
				this->iterator = Move(rhs.iterator);
				this->path = Move(rhs.path);
			}

			inline WalkerState &operator=(WalkerState &&rhs)
			{
				this->iterator = Move(rhs.iterator);
				this->path = Move(rhs.path);

				return *this;
			}

			WalkerState(const WalkerState&) = delete;
			WalkerState &operator=(const WalkerState &rhs) = delete;

			inline bool operator==(const WalkerState &rhs) const
			{
				return *this->iterator == *rhs.iterator;
			}
		};
	public:
		//Constructors
		inline DirectoryWalker(const ReadableFileSystem& fileSystem) : fileSystem(fileSystem)
		{
		}

		inline DirectoryWalker(const ReadableFileSystem& fileSystem, const Path& path) : fileSystem(fileSystem)
		{
			this->AddState(path);
			this->CorrectIteratorPos();
		}

		//Inline operators
		inline bool operator==(const DirectoryWalker &rhs) const
		{
			if(this->states.IsEmpty())
				return rhs.states.IsEmpty();
			if(rhs.states.IsEmpty())
				return false;

			return this->states.Last() == rhs.states.Last();
		}

		inline bool operator!=(const DirectoryWalker &rhs) const
		{
			return !(*this == rhs);
		}

		inline Path operator*()
		{
			WalkerState& topState = this->states.Last();
			DirectoryEntry s = *(*topState.iterator);
			if(topState.path.String().IsEmpty())
				return s.name;
			return topState.path / s.name;
		}

		inline DirectoryWalker &operator++() //Prefix++
		{
			this->states.Last().iterator->operator++();
			this->CorrectIteratorPos();
			return *this;
		}

	private:
		//Members
		const ReadableFileSystem& fileSystem;
		LinkedList<WalkerState> states;

		//Methods
		void CorrectIteratorPos();

		//Inline
		inline void AddState(const Path& path)
		{
			UniquePointer<DirectoryEnumerator> enumerator = this->fileSystem.EnumerateChildren(path);
			this->states.InsertTail(WalkerState(new DirectoryIterator(Move(enumerator)), path));
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
		inline DirectoryWalker begin() const
		{
			return DirectoryWalker(this->fileSystem, this->path);
		}

		inline DirectoryWalker end() const
		{
			return DirectoryWalker(this->fileSystem);
		}

	private:
		//Members
		const ReadableFileSystem& fileSystem;
		Path path;
	};
}
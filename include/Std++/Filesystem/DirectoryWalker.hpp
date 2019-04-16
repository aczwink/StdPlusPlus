/*
 * Copyright (c) 2018-2019 Amir Czwink (amir130@hotmail.de)
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
#include "Directory.hpp"
#include "DirectoryIterator.hpp"
#include "File.hpp"

namespace StdXX
{
	class STDPLUSPLUS_API DirectoryWalker
	{
		struct WalkerState
		{
			AutoPointer<const Directory> directory;
			AutoPointer<DirectoryIterator> iterator;
			Path path;

			/*inline WalkerState()
			{
			}*/

			inline WalkerState(AutoPointer<const Directory> &&directory, AutoPointer<DirectoryIterator> &&iterator, const Path& path) : path(path)
			{
				this->directory = Move(directory);
				this->iterator = Move(iterator);
			}

			WalkerState(const WalkerState&) = default;

			inline WalkerState(WalkerState &&rhs)
			{
				this->directory = Move(rhs.directory);
				this->iterator = Move(rhs.iterator);
				this->path = Move(rhs.path);
			}

			inline WalkerState &operator=(const WalkerState &rhs)
			{
				this->directory = rhs.directory;
				this->iterator = rhs.iterator;
				this->path = rhs.path;

				return *this;
			}

			inline WalkerState &operator=(WalkerState &&rhs)
			{
				this->directory = Move(rhs.directory);
				this->iterator = Move(rhs.iterator);
				this->path = Move(rhs.path);

				return *this;
			}

			inline bool operator==(const WalkerState &rhs) const
			{
				return this->directory == rhs.directory && *this->iterator == *rhs.iterator;
			}
		};
	public:
		//Constructor
		inline DirectoryWalker(AutoPointer<const Directory> directory)
		{
			this->AddState(directory, String(u8""));
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

		inline Tuple<Path, AutoPointer<File>> operator*()
		{
			WalkerState& topState = this->states.Last();
			Tuple<String, AutoPointer<FileSystemNode>> t = *(*topState.iterator);
			if(topState.path.GetString().IsEmpty())
				return { t.Get<0>(), t.Get<1>().MoveCast<File>() };
			return { topState.path / t.Get<0>(), t.Get<1>().MoveCast<File>() };
		}

		inline DirectoryWalker &operator++() //Prefix++
		{
			this->states.Last().iterator->operator++();
			this->CorrectIteratorPos();
			return *this;
		}

	private:
		//Members
		LinkedList<WalkerState> states;

		//Methods
		void CorrectIteratorPos();

		//Inline
		inline void AddState(AutoPointer<const Directory> directory, const Path& path)
		{
			if(!directory.IsNull())
			{
				DirectoryIterator it = directory->begin();
				this->states.InsertTail(WalkerState(Move(directory), new DirectoryIterator(Move(it)), path));
			}
		}
	};

	class DirectoryWalkerWrapper
	{
	public:
		//Constructor
		inline DirectoryWalkerWrapper(AutoPointer<const Directory> directory) : directory(directory)
		{
		}

		//For range-based loop
		inline DirectoryWalker begin() const
		{
			return DirectoryWalker(this->directory);
		}

		inline DirectoryWalker end() const
		{
			return DirectoryWalker(nullptr);
		}

	private:
		//Members
		AutoPointer<const Directory> directory;
	};
}
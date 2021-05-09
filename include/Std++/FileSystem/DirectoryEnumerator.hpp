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
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Containers/Enumeration/Enumerator.hpp>
#include "FileInfo.hpp"

namespace StdXX::FileSystem
{
	struct DirectoryEntry
	{
		String name;
		FileType type;
	};

	class DirectoryEnumerator : public Enumerator<DirectoryEntry>
	{
	};

	class SelfAndParentFiltereredDirectoryEnumerator : public DirectoryEnumerator
	{
	public:
		//Constructor
		SelfAndParentFiltereredDirectoryEnumerator(UniquePointer<DirectoryEnumerator>&& enumerator) : enumerator(Move(enumerator))
		{
		}

		//Methods
		const DirectoryEntry &GetCurrent() const override
		{
			return this->enumerator->GetCurrent();
		}

		bool MoveForward() override
		{
			bool ret = this->enumerator->MoveForward();
			if(ret and ((this->enumerator->GetCurrent().name == u8".") || (this->enumerator->GetCurrent().name == u8"..")))
				return this->MoveForward();
			return ret;
		}

	private:
		//Members
		UniquePointer<DirectoryEnumerator> enumerator;
	};
}
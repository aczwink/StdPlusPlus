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
//Class header
#include <Std++/FileSystem/DirectoryWalker.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

//Private methods
bool DirectoryWalker::Next(Path &entry)
{
	DirectoryEntry directoryEntry;

	while(!this->states.IsEmpty())
	{
		WalkerState& topState = this->states.Last();
		if(!topState.enumerator->Next(directoryEntry))
		{
			this->states.PopTail();
			continue;
		}

		if(directoryEntry.type == FileType::Directory)
		{
			Path subPath;
			if(topState.path.String().IsEmpty())
				subPath = directoryEntry.name;
			else
				subPath = topState.path / directoryEntry.name;

			this->AddState(subPath);
		}
		else
		{
			if(topState.path.String().IsEmpty())
				entry = directoryEntry.name;
			else
				entry = topState.path / directoryEntry.name;
			return true; //fine, we have a file
		}
	}

	return false;
}
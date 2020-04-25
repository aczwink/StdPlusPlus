/*
 * Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
void DirectoryWalker::CorrectIteratorPos()
{
	while(!this->states.IsEmpty())
	{
		WalkerState &state = this->states.Last();
		if(*state.iterator == state.directory->end())
		{
			this->states.PopTail();
			continue;
		}

		while(*state.iterator != state.directory->end())
		{
			String childName = state.iterator->operator*();
			if (state.directory->HasSubDirectory(childName))
			{
				AutoPointer<const Directory> subdir = state.directory->GetSubDirectory(childName);
				Path subPath;
				if(state.path.String().IsEmpty())
					subPath = childName;
				else
					subPath = state.path / childName;

				state.iterator->operator++(); //advance the sub dir on current level
				if(*state.iterator == state.directory->end())
					this->states.PopTail();

				this->AddState(Move(subdir), Move(subPath)); //add sub dir
				break; //check new state
			} else
				return; //fine, we have a file
		}
	}
}
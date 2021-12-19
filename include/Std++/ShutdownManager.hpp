/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Array/DynamicArray.hpp>

namespace StdXX
{
	class Releasable
	{
	public:
		virtual void Release() = 0;
	};

	class ShutdownManager
	{
	public:
		//Functions
		static ShutdownManager& Instance()
		{
			static ShutdownManager instance;
			return instance;
		}

		//Inline
		inline void Register(Releasable* releasable)
		{
			this->releasables.Push(releasable);
		}

		inline void Shutdown()
		{
			for(Releasable* releasable : this->releasables)
				releasable->Release();
			this->releasables.Release();
		}

	private:
		//Members
		DynamicArray<Releasable*> releasables;
		//Constructor
		ShutdownManager() = default;
	};
}
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
#include "Format.hpp"
#include "RWFileSystem.hpp"

namespace StdXX::FileSystem
{
	//Forward declarations
	class OSFileSystem;

	class FileSystemsManager
	{
	public:
		//Methods
		class OSFileSystem& OSFileSystem();

		//Functions
		static inline FileSystemsManager& Instance()
		{
			static FileSystemsManager instance;
			return instance;
		}

		//Inline
		inline void ReleaseAllFormats()
		{
			for(const Format *format : this->fsFormats)
				delete(format);
			this->fsFormats.Release();
		}

	private:
		//Members
		DynamicArray<const Format *> fsFormats;

		//Constructor
		FileSystemsManager() = default;
	};
}
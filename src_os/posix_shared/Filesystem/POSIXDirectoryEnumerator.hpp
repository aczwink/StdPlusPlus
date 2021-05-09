/*
 * Copyright (c) 2019-2021 Amir Czwink (amir130@hotmail.de)
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
//Global
#include <dirent.h>
//Local
#include <Std++/FileSystem/DirectoryEnumerator.hpp>
#include <Std++/FileSystem/Path.hpp>

namespace _stdxx_
{
	class POSIXDirectoryEnumerator : public StdXX::FileSystem::DirectoryEnumerator
	{
	public:
		//Constructor
		POSIXDirectoryEnumerator(const StdXX::FileSystem::Path& path);

		//Destructor
		~POSIXDirectoryEnumerator();

		//Methods
		const StdXX::FileSystem::DirectoryEntry &GetCurrent() const override;
		bool MoveForward() override;

	private:
		//Members
		DIR* dir;
		StdXX::FileSystem::DirectoryEntry directoryEntry;
	};
}
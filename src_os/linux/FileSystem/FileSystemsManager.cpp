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
#include <Std++/FileSystem/FileSystemsManager.hpp>
//Local
#include <Std++/FileSystem/OSFileSystem.hpp>
#include "../../posix_shared/Filesystem/POSIXFileSystem.hpp"

class OSFileSystem &FileSystemsManager::OSFileSystem()
{
	static class LinuxFileSystem : public _stdxx_::POSIXFileSystem
	{
		void HandleNonCompliantUnlinkError() override
		{
			if(errno == EISDIR)
				NOT_IMPLEMENTED_ERROR; //TODO: linux reports EISDIR for directories
			else
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	} linux_fs;

	return linux_fs;
}
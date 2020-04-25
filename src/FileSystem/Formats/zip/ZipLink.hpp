/*
* Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/FileSystem/Link.hpp>
#include "ZipReadableFile.hpp"

namespace _stdxx_
{
	class ZipLink : public StdXX::FileSystem::Link, public ZipReadableFile
	{
	public:
		//Constructor
		inline ZipLink(const CentralDirectoryRecord& centralDirectoryRecord, ZipFileSystem& fileSystem)
				: ZipReadableFile(centralDirectoryRecord, fileSystem)
		{
		}

		//Methods
		void ChangePermissions(const StdXX::FileSystem::NodePermissions &newPermissions) override;
		StdXX::NodeInfo QueryInfo() const override;
		StdXX::FileSystem::Path ReadTarget() const override;
	};
}
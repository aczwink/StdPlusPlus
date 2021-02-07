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
#pragma once
//Local
#include <Std++/FileSystem/File.hpp>
#include <Std++/Streams/SeekableInputStream.hpp>
#include <Std++/Compression/CompressionAlgorithm.hpp>
#include "Zip.hpp"
#include "CentralDirectoryRecord.hpp"
#include "ZipReadableFile.hpp"

namespace _stdxx_
{
	//Forward declarations
	class ZipFileSystem;

	class ZipFile : public StdXX::FileSystem::File, public ZipReadableFile
	{
	public:
		//Constructor
		inline ZipFile(const CentralDirectoryRecord& centralDirectoryRecord, ZipFileSystem& fileSystem)
			: ZipReadableFile(centralDirectoryRecord, fileSystem)
		{
		}

		//Methods
		void ChangePermissions(const StdXX::FileSystem::Permissions &newPermissions) override;
		StdXX::UniquePointer<StdXX::InputStream> OpenForReading(bool verify) const override;
		StdXX::UniquePointer<StdXX::OutputStream> OpenForWriting() override;
		StdXX::NodeInfo QueryInfo() const override;
	};
}
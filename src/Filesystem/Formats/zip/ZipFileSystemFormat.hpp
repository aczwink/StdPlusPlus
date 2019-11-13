/*
* Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Filesystem/FileSystemFormat.hpp>
#include <Std++/Filetypes/UTIConstants.hpp>
#include <Std++/Buffers/FixedSizeBuffer.hpp>
#include <Std++/Streams/BufferInputStream.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Streams/FileInputStream.hpp>
#include "Zip.hpp"
#include "ZipFileSystem.hpp"

namespace _stdxx_
{
	class ZipFileSystemFormat : public StdXX::FileSystemFormat
	{
	public:
		//Methods
		StdXX::FileSystem* CreateFileSystem(const StdXX::Path &fileSystemPath) const override;

		StdXX::String GetId() const override
		{
			return StdXX::FileTypes::UTI::zip;
		}

		StdXX::String GetName() const override
		{
			return u8"ZIP archive";
		}

		float32 Matches(StdXX::SeekableInputStream &inputStream) const override
		{
			if(this->FindEndOfCentralDirectoryOffset(inputStream) != StdXX::Unsigned<uint64>::Max())
				return 1;
			return 0;
		}

		StdXX::FileSystem *OpenFileSystem(const StdXX::Path &fileSystemPath, bool writable) const override;

	private:
		//Methods
		uint64 FindEndOfCentralDirectoryOffset(StdXX::SeekableInputStream& inputStream) const;
		bool ValidateEndOfCentralDirectory(const StdXX::FixedSizeBuffer& buffer, uint32 offset) const;
	};
}
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
#include <Std++/Filesystem/Path.hpp>
#include <Std++/Streams/FileInputStream.hpp>
#include <Std++/Filesystem/BufferedMetadataFileSystem.hpp>
#include <Std++/Multitasking/Mutex.hpp>
#include <Std++/Streams/FileUpdateStream.hpp>
#include "Zip.hpp"
#include "ZipDirectory.hpp"
#include "EndOfCentralDirectory.hpp"
#include "ZipFileSystemFormat.hpp"

namespace _stdxx_
{
	/**
	 * Implementation based on: APPNOTE.TXT - .ZIP File Format Specification Version: 6.3.6
	 */
	class ZipFileSystem : public StdXX::BufferedMetadataFileSystem
	{
	public:
		//Constructors
		ZipFileSystem(const StdXX::FileSystemFormat *format, const StdXX::Path &path);
		ZipFileSystem(const StdXX::FileSystemFormat *format, const StdXX::Path& path, uint64 endOfCentralDirectoryOffset, bool writable);

//Properties
		inline StdXX::SeekableInputStream& InputStream()
		{
			if(this->readOnlyInputStream.IsNull())
				return *this->writableStream;
			return *this->readOnlyInputStream;
		}

		inline StdXX::Mutex& StreamLock()
		{
			return this->streamLock;
		}

		//Methods
		StdXX::UniquePointer<StdXX::OutputStream> CreateFile(const StdXX::Path &filePath) override;
		bool Exists(const StdXX::Path &path) const override;
		void Flush() override;
		StdXX::AutoPointer<StdXX::Directory> GetRoot() override;
		StdXX::AutoPointer<const StdXX::Directory> GetRoot() const override;
		uint64 GetSize() const override;
		void Move(const StdXX::Path &from, const StdXX::Path &to) override;

	private:
		//Members
		StdXX::UniquePointer<StdXX::FileInputStream> readOnlyInputStream;
		StdXX::UniquePointer<StdXX::FileUpdateStream> writableStream;
		StdXX::Mutex streamLock;
		StdXX::AutoPointer<ZipDirectory> root;

		//Methods
		/**
		 * Based on sections:
		 * 4.3.12
		 * APPENDIX D - Language Encoding (EFS)
		 * @param record
		 */
		void ReadCentralDirectory(const EndOfCentralDirectory& record);
		/**
		 * Based on sections:
		 * 4.3.16
		 * @param offset
		 * @return
		 */
		EndOfCentralDirectory ReadEndOfCentralDirectory(uint64 offset);
	};
}
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
#pragma once
//Local
#include <Std++/FileSystem/Path.hpp>
#include <Std++/Streams/FileInputStream.hpp>
#include <Std++/FileSystem/BufferedMetadataFileSystem.hpp>
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
	class ZipFileSystem : public StdXX::FileSystem::BufferedMetadataFileSystem
	{
	public:
		//Constructors
		ZipFileSystem(const StdXX::FileSystem::Path &path);
		ZipFileSystem(const StdXX::FileSystem::Path& path, const StdXX::FileSystem::OpenOptions& openOptions, uint64 endOfCentralDirectoryOffset, bool writable);

		//Properties
		inline StdXX::SeekableInputStream& InputStream()
		{
			if(this->readOnlyInputStream.IsNull())
				return *this->writableStream;
			return *this->readOnlyInputStream;
		}

		inline const StdXX::String& Password() const
		{
			return this->password;
		}

		inline StdXX::Mutex& StreamLock()
		{
			return this->streamLock;
		}

		//Methods
		void CreateLink(const StdXX::FileSystem::Path &linkPath, const StdXX::FileSystem::Path &linkTargetPath) override;
		bool Exists(const StdXX::FileSystem::Path &path) const override;
		void Flush() override;
		StdXX::AutoPointer<StdXX::FileSystem::Directory> GetRoot() override;
		StdXX::AutoPointer<const StdXX::FileSystem::Directory> GetRoot() const override;
		void Move(const StdXX::FileSystem::Path &from, const StdXX::FileSystem::Path &to) override;
		StdXX::FileSystem::SpaceInfo QuerySpace() const override;

		//Inline
		inline void InformNodeChanged()
		{
			this->isFlushed = false;
		}

	private:
		//Members
		bool isFlushed;
		StdXX::UniquePointer<StdXX::FileInputStream> readOnlyInputStream;
		StdXX::UniquePointer<StdXX::FileUpdateStream> writableStream;
		StdXX::Mutex streamLock;
		StdXX::AutoPointer<ZipDirectory> root;
		StdXX::String password;

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
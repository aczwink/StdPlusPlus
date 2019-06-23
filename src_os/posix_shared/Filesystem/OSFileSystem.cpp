/*
 * Copyright (c) 2018-2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Filesystem/OSFileSystem.hpp>
//Global
#include <climits>
#include <cstdio>
#include <sys/stat.h>
#include <unistd.h>
//Local
#include "POSIXDirectory.hpp"
#include "POSIXFile.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Public methods
Path OSFileSystem::FromNativePath(const String &nativePath) const
{
	//no changes needed here
	return nativePath;
}

Path OSFileSystem::GetWorkingDirectory() const
{
	char buffer[4096];
	char* ret = getcwd(buffer, sizeof(buffer));
	ASSERT(ret, u8"REPORT THIS PLEASE!");

	return String::CopyRawString(buffer);
}

Path OSFileSystem::ToAbsolutePath(const Path &path) const
{
	char p[PATH_MAX];
	realpath(reinterpret_cast<const char *>(path.GetString().ToUTF8().GetRawZeroTerminatedData()), p);

	return {String::CopyRawString(p)};
}

//Class functions
OSFileSystem &OSFileSystem::GetInstance()
{
	static class POSIX_FS : public OSFileSystem
	{
	public:
		//Public methods
		UniquePointer<OutputStream> CreateFile(const Path &filePath) override
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return nullptr;
		}

		bool Exists(const Path &path) const override
		{
			Path p = this->ToAbsolutePath(path);
			struct stat sb{};
			return stat(reinterpret_cast<const char *>(p.GetString().ToUTF8().GetRawZeroTerminatedData()), &sb) == 0;
		}

		void Flush() override
		{
		}

		AutoPointer<FileSystemNode> GetNode(const Path &path) override
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return AutoPointer<FileSystemNode>();
		}

		AutoPointer<const FileSystemNode> GetNode(const Path &path) const override
		{
			Path p = this->ToAbsolutePath(path);
			struct stat sb{};
			int ret = stat(reinterpret_cast<const char *>(p.GetString().ToUTF8().GetRawZeroTerminatedData()), &sb);
			ASSERT(ret == 0, u8"TODO: node does not exist. Throw exception");

			if(S_ISDIR(sb.st_mode))
				return new POSIXDirectory(p);
			if(S_ISREG(sb.st_mode))
				return new POSIXFile(p);

			NOT_IMPLEMENTED_ERROR;
			return nullptr;
		}

		AutoPointer<Directory> GetRoot() override
		{
			return new POSIXDirectory(Path(u8"/"));
		}

		uint64 GetSize() const override
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return 0;
		}

		void Move(const Path &from, const Path &to) override
		{
			rename(reinterpret_cast<const char *>(from.GetString().ToUTF8().GetRawZeroTerminatedData()),
				   reinterpret_cast<const char *>(to.GetString().ToUTF8().GetRawZeroTerminatedData()));
		}
	} posix_fs;

	return posix_fs;
}
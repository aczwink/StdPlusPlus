/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Filesystem/FileSystem.hpp>
//Global
#include <stdio.h>
#include <sys/stat.h>
//Local
#include <Std++/Filesystem/DirectoryIterator.hpp>
//Namespaces
using namespace StdXX;

class LinuxDirectory : public Directory
{
public:
	//Constructor
	inline LinuxDirectory(const Path &path) : path(path)
	{
	}

	//Methods
	bool ContainsFile(const String &name) const override
	{
		Path p = this->path / name;
		struct stat sb{};
		return stat(reinterpret_cast<const char *>(p.GetString().ToUTF8().GetRawZeroTerminatedData()), &sb) == 0 && S_ISDIR(sb.st_mode) == 0;
	}

	bool ContainsSubDirectory(const String &name) const override
	{
		Path p = this->path / name;
		struct stat sb{};
		return stat(reinterpret_cast<const char *>(p.GetString().ToUTF8().GetRawZeroTerminatedData()), &sb) == 0 && S_ISDIR(sb.st_mode) != 0;
	}

	UniquePointer<OutputStream> CreateFile(const String &name) override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return nullptr;
	}

	void CreateSubDirectory(const String &name) override
	{
		Path p = this->path / name;
		bool success = mkdir(reinterpret_cast<const char *>(p.GetString().ToUTF8().GetRawZeroTerminatedData()), 0700) == 0;
	}

	bool Exists(const Path &path) const override
	{
		Path p = this->path / path;
		struct stat sb{};
		return stat(reinterpret_cast<const char *>(p.GetString().ToUTF8().GetRawZeroTerminatedData()), &sb) == 0;
	}

	FileSystem *GetFileSystem() override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return nullptr;
	}

	const FileSystem *GetFileSystem() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return nullptr;
	}

	AutoPointer<Directory> GetSubDirectory(const String &name) override
	{
		return new LinuxDirectory(this->path / name);
	}

	AutoPointer<const Directory> GetSubDirectory(const String &name) const override
	{
		return new LinuxDirectory(this->path / name);
	}

	DirectoryIterator begin() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return nullptr;
	}

	DirectoryIterator end() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return nullptr;
	}

	String GetName() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return nullptr;
	}

	AutoPointer<Directory> GetParent() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return nullptr;
	}

	Path GetPath() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return Path();
	}

	uint64 GetSize() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return 0;
	}

private:
	//Members
	Path path;
};

//Class functions
FileSystem &FileSystem::GetOSFileSystem()
{
	static class LinuxFS : public FileSystem
	{
	public:
		//Constructor
		LinuxFS() : FileSystem(nullptr)
		{}

		//Public methods
		UniquePointer<OutputStream> CreateFile(const Path &filePath) override
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return nullptr;
		}

		bool Exists(const Path &path) const override
		{
			Path p = path.GetAbsolutePath();
			struct stat sb{};
			return stat(reinterpret_cast<const char *>(p.GetString().ToUTF8().GetRawZeroTerminatedData()), &sb) == 0;
		}

		void Flush() override
		{
		}

		AutoPointer<Directory> GetDirectory(const Path &directoryPath) override
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return nullptr;
		}

		AutoPointer<Directory> GetRoot() override
		{
			return new LinuxDirectory(Path(u8"/"));
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
	} linuxFS;

	return linuxFS;
}
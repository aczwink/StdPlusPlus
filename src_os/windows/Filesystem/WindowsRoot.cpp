/*
* Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
#include "WindowsRoot.hpp"
//Global
#include <Windows.h>
#undef CreateFile
//Local
#include <Std++/Filesystem/DirectoryIterator.hpp>
#include "WindowsDirectory.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

class WindowsDrivesIteratorState : public DirectoryIteratorState
{
public:
	//Constructor
	inline WindowsDrivesIteratorState()
	{
		this->driveMask = GetLogicalDrives();
		this->current = 1;
		if (!(this->current & this->driveMask))
			this->Next();
	}

	//Public methods
	bool Equals(DirectoryIteratorState * other) const override
	{
		if ((other == nullptr) && (this->current == (1 << 26)))
			return true;
		if (IS_INSTANCE_OF(other, WindowsDrivesIteratorState))
		{
			WindowsDrivesIteratorState* otherTyped = dynamic_cast<WindowsDrivesIteratorState*>(other);
			return otherTyped->current == this->current;
		}
		return false;
	}

	Tuple<String, AutoPointer<FileSystemNode>> GetCurrent() override
	{
		String letter;
		for (uint8 i = 0; i < 26; i++)
		{
			if (this->current == (1 << i))
			{
				letter += (u8'A' + i);
				letter += u8":";
				break;
			}
		}

		return { letter, new WindowsDirectory(u8"/" + letter) };
	}

	void Next() override
	{
		this->current <<= 1;
		while (!(driveMask & this->current) && (this->current < (1 << 26)))
			this->current <<= 1;
	}

private:
	//Members
	DWORD driveMask;
	DWORD current;
};

//Public methods
bool WindowsRoot::ContainsFile(const String & name) const
{
	//root has no files
	return false;
}

bool WindowsRoot::ContainsSubDirectory(const String & name) const
{
	if (name.GetLength() != 2)
		return false;
	if (name.SubString(1, 1) != u8":")
		return false;

	DWORD driveMask = GetLogicalDrives();
	for (uint8 i = 0; i < 26; i++)
	{
		if (driveMask & (1 << i))
		{
			String letter;
			letter += (u8'A' + i);
			letter += u8":";
			if (name == letter)
				return true;
		}
	}
	return false;
}

//Range-based loops
DirectoryIterator WindowsRoot::begin() const
{
	return DirectoryIterator(new WindowsDrivesIteratorState);
}

DirectoryIterator WindowsRoot::end() const
{
	return DirectoryIterator(nullptr);
}



//NOT IMPLEMENTED
UniquePointer<OutputStream> _stdxx_::WindowsRoot::CreateFile(const String & name)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return UniquePointer<OutputStream>();
}

void _stdxx_::WindowsRoot::CreateSubDirectory(const String & name)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

bool _stdxx_::WindowsRoot::Exists(const Path & path) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return false;
}

AutoPointer<const File> _stdxx_::WindowsRoot::GetFile(const String & name) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return AutoPointer<const File>();
}

FileSystem * _stdxx_::WindowsRoot::GetFileSystem()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

const FileSystem * _stdxx_::WindowsRoot::GetFileSystem() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

AutoPointer<const Directory> _stdxx_::WindowsRoot::GetParent() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return AutoPointer<const Directory>();
}

Path _stdxx_::WindowsRoot::GetPath() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return Path();
}

AutoPointer<Directory> _stdxx_::WindowsRoot::GetSubDirectory(const String & name)
{
	if (this->ContainsSubDirectory(name))
		return new WindowsDirectory(u8"/" + name);
	return nullptr;
}

AutoPointer<const Directory> _stdxx_::WindowsRoot::GetSubDirectory(const String & name) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return AutoPointer<const Directory>();
}

uint64 _stdxx_::WindowsRoot::GetStoredSize() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return uint64();
}

uint64 _stdxx_::WindowsRoot::GetSize() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return uint64();
}
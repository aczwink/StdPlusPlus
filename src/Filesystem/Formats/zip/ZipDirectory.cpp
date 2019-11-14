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
//Class header
#include "ZipDirectory.hpp"
//Local
#include <Std++/Filesystem/DirectoryIterator.hpp>
#include "ZipFileSystem.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Public methods
void ZipDirectory::CreateSubDirectory(const String &name)
{
	this->AddChild(name, new ZipDirectory(this->fileSystem));

	this->fileSystem.InformNodeChanged();
}

//TODO NOT IMPLEMENTED
StdXX::UniquePointer<StdXX::OutputStream> _stdxx_::ZipDirectory::CreateFile(const StdXX::String &name) {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::UniquePointer<StdXX::OutputStream>();
}

bool _stdxx_::ZipDirectory::Exists(const StdXX::Path &path) const {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return false;
}

StdXX::FileSystem *_stdxx_::ZipDirectory::GetFileSystem() {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

const StdXX::FileSystem *_stdxx_::ZipDirectory::GetFileSystem() const {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

StdXX::AutoPointer<const StdXX::Directory> _stdxx_::ZipDirectory::GetParent() const {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::AutoPointer<const Directory>();
}

StdXX::Path _stdxx_::ZipDirectory::GetPath() const {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::Path();
}

bool _stdxx_::ZipDirectory::IsEmpty() const {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return false;
}

StdXX::FileSystemNodeInfo _stdxx_::ZipDirectory::QueryInfo() const {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::FileSystemNodeInfo();
}
//END TODO NOT IMPLEMENTED
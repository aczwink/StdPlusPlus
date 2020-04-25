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
//Class header
#include "ZipDirectory.hpp"
//Local
#include <Std++/FileSystem/DirectoryIterator.hpp>
#include "ZipFileSystem.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::FileSystem;

//Public methods
void ZipDirectory::ChangePermissions(const FileSystem::NodePermissions &newPermissions)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

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

bool _stdxx_::ZipDirectory::Exists(const Path &path) const {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return false;
}

bool _stdxx_::ZipDirectory::IsEmpty() const {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return false;
}

StdXX::NodeInfo _stdxx_::ZipDirectory::QueryInfo() const {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::NodeInfo();
}
//END TODO NOT IMPLEMENTED
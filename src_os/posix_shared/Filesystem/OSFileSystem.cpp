/*
 * Copyright (c) 2018-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/FileSystem/OSFileSystem.hpp>
//Global
#include <stdio.h>
#include <unistd.h>
//Local
#include "../src_backends/fuse3/fuse3.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::FileSystem;

//Public methods
Path OSFileSystem::FromNativePath(const String &nativePath) const
{
	//no changes needed here
	return nativePath;
}

Path OSFileSystem::GetTempPath() const
{
	return String(P_tmpdir);
}

Path OSFileSystem::GetWorkingDirectory() const
{
	char buffer[4096];
	char* ret = getcwd(buffer, sizeof(buffer));
	ASSERT(ret, u8"REPORT THIS PLEASE!");

	return String::CopyRawString(buffer);
}

void OSFileSystem::MountReadOnly(const Path &mountPoint, const ReadableFileSystem &fileSystem)
{
#ifdef _STDXX_EXTENSION_FUSE3
	fuse_MountReadOnly(mountPoint, fileSystem);
	return;
#endif
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

Path OSFileSystem::ToAbsolutePath(const Path &path) const
{
	//realpath in general works well but unfortunately resolves symlinks
	/*
	char p[PATH_MAX];
	realpath(reinterpret_cast<const char *>(path.String().ToUTF8().GetRawZeroTerminatedData()), p);
	return {String::CopyRawString(p)};
	 */
	if(path.IsAbsolute())
		return path.Normalized();
	return (this->GetWorkingDirectory() / path).Normalized();
}
/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include <ACStdLib/Filesystem/Path.hpp>
//Global
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
//Local
#include <ACStdLib/Containers/Strings/UTF-8/UTF8String.hpp>
//Namespaces
using namespace ACStdLib;

//Public methods
/* TODO: new file system implementation
bool Path::CreateDirectory()
{
	return mkdir(reinterpret_cast<const char *>(this->pathString.ToUTF8().GetRawZeroTerminatedData()), 0700) == 0;
}

bool Path::Exists() const
{
	return access(reinterpret_cast<const char *>(this->pathString.ToUTF8().GetRawZeroTerminatedData()), F_OK) == 0;
}
*/

Path Path::GetAbsolutePath() const
{
	char p[PATH_MAX];
	realpath(reinterpret_cast<const char *>(this->pathString.ToUTF8().GetRawZeroTerminatedData()), p);

	return {String::CopyRawString(p)};
}

bool Path::IsDirectory() const
{
	struct stat path_stat;

	if(stat(reinterpret_cast<const char *>(this->pathString.ToUTF8().GetRawZeroTerminatedData()), &path_stat) != 0)
		return false;

	return S_ISDIR(path_stat.st_mode);
}
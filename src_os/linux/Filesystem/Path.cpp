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
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
//Local
#include <ACStdLib/Containers/Strings/UTF-8/UTF8String.hpp>
//Namespaces
using namespace ACStdLib;

//Public methods
bool Path::Exists() const
{
	UTF8String pathUTF8(this->pathString.GetUTF16());

	return access((const char *)pathUTF8.GetC_Str(), F_OK) == 0;
}

bool Path::IsDirectory() const
{
	struct stat path_stat;
	UTF8String pathUTF8(this->pathString.GetUTF16());

	if(stat((const char *)pathUTF8.GetC_Str(), &path_stat) != 0)
		return false;

	return S_ISDIR(path_stat.st_mode);
}
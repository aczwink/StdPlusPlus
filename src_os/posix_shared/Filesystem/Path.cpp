/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Filesystem/Path.hpp>
//Global
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
//Local
#include <Std++/Containers/Strings/UTF-8/UTF8String.hpp>
//Namespaces
using namespace StdPlusPlus;

//Public methods
Path Path::GetAbsolutePath() const
{
	char p[PATH_MAX];
	realpath(reinterpret_cast<const char *>(this->pathString.ToUTF8().GetRawZeroTerminatedData()), p);

	return {String::CopyRawString(p)};
}
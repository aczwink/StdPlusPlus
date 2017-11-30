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
#include <ACStdLib/Filesystem/PathIterator.hpp>
//Global
#include <dirent.h>
//Local
#include <ACStdLib/ErrorHandling/FileNotFoundException.hpp>
//Namespaces
using namespace ACStdLib;
//Definitions
#define THIS ((DIR *)this->pOSHandle)

//Constructor
PathIterator::PathIterator(const Path &path, bool end)
{
	if(end)
	{
		this->pOSHandle = nullptr;
		return;
	}

	UTF8String pathUTF8 = path.GetString().GetUTF16();
	this->pOSHandle = opendir(reinterpret_cast<const char *>(pathUTF8.GetC_Str()));
	if(!this->pOSHandle)
		throw ErrorHandling::FileNotFoundException(path);

	++(*this);
}

//Destructor
PathIterator::~PathIterator()
{
	if(this->pOSHandle)
		closedir(THIS);
}

//Operators
PathIterator &PathIterator::operator++()
{
	dirent *ent = readdir(THIS);
	while(ent) //unfortunately, the order of the returned files is not known. Therefore we don't know when "." and ".." will arrive...
	{
		if(!(ent->d_name[0] == '.' && ((ent->d_name[1] == 0) || (ent->d_name[1] == '.' && ent->d_name[2] == 0))))
		{
			this->currentPath = ent->d_name;
			return *this;
		}

		ent = readdir(THIS);
	}

	closedir(THIS);
	this->pOSHandle = nullptr;

	return *this;
}

bool PathIterator::operator!=(const PathIterator &other) const
{
	return this->pOSHandle != other.pOSHandle;
}
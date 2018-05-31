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
#include <Std++/Filesystem/DirectoryIterator.hpp>
//Global
#include <dirent.h>
//Local
#include <Std++/Errorhandling/FileNotFoundException.hpp>
//Namespaces
using namespace StdPlusPlus;
//Definitions
#define THIS ((DIR *)this->iteratorState)

/*
//Constructor
DirectoryIterator::DirectoryIterator(_stdpp::DirectoryIteratorState *iteratorState)
{
	if(end)
	{
		this->iteratorState = nullptr;
		return;
	}

	this->iteratorState = opendir(reinterpret_cast<const char *>(path.GetString().ToUTF8().GetRawZeroTerminatedData()));
	if(!this->iteratorState)
		throw ErrorHandling::FileNotFoundException(path);

	++(*this);
}
*/

/*
//Destructor
DirectoryIterator::~DirectoryIterator()
{
	if(this->iteratorState)
		closedir(THIS);
}
 */

//Operators
/*
DirectoryIterator &DirectoryIterator::operator++()
{
	dirent *ent = readdir(THIS);
	while(ent) //unfortunately, the order of the returned files is not known. Therefore we don't know when "." and ".." will arrive...
	{
		if(!(ent->d_name[0] == '.' && ((ent->d_name[1] == 0) || (ent->d_name[1] == '.' && ent->d_name[2] == 0))))
		{
			this->currentPath = String::CopyRawString(ent->d_name);
			return *this;
		}

		ent = readdir(THIS);
	}

	closedir(THIS);
	this->iteratorState = nullptr;

	return *this;
}
 */
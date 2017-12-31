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
//Local
#include <ACStdLib/Filesystem/PathIterator.hpp>
//Namespaces
using namespace ACStdLib;

//Public methods
bool Path::CreateDirectoryTree()
{
	if(this->Exists())
	{
		if(this->IsDirectory())
			return true;

		return false;
	}

	Path parent = this->GetParent();
	if(parent == parent.GetRoot() || parent.CreateDirectoryTree()) //create parent directory
	{
		return this->CreateDirectory();
	}

	return false;
}

String Path::GetFileExtension() const
{
	uint32 pos;

	pos = this->pathString.FindReverse(u8".");

	if(pos == Natural<uint32>::Max())
		return String();

	return this->pathString.SubString(pos + 1, this->pathString.GetLength() - pos - 1).ToLowercase();
}

String Path::GetName() const
{
	uint32 pos;

	pos = this->pathString.FindReverse(u8"/");

	if(pos == Natural<uint32>::Max())
		return this->pathString;

	return this->pathString.SubString(pos + 1, this->pathString.GetLength() - pos - 1);
}

Path Path::GetParent() const
{
	uint32 pos;

	pos = this->pathString.FindReverse(u8"/");
	if(pos == Natural<uint32>::Max())
		return Path();

	return Path(this->pathString.SubString(0, pos));
}

Path Path::GetRoot() const
{
	uint32 pos;

	pos = this->pathString.Find(u8"/");
	if(pos == Natural<uint32>::Max())
		return Path();

	return Path(this->pathString.SubString(0, pos + 1));
}

String Path::GetTitle() const
{
	uint32 posDot, posSlash;

	posDot = this->pathString.FindReverse(u8".");
	posSlash = this->pathString.FindReverse(u8"/", posDot);

	if(posSlash == Natural<uint32>::Max())
	{
		if(posDot == Natural<uint32>::Max())
			return this->pathString;
		return this->pathString.SubString(0, posDot);
	}
	if(posDot == Natural<uint32>::Max())
		return this->pathString.SubString(posSlash + 1, this->pathString.GetLength() - posSlash - 1);
	if(posSlash > posDot)
		return String();

	return this->pathString.SubString(posSlash + 1, posDot - posSlash - 1);
}

//For range-based loop
PathIterator Path::begin() const
{
	return PathIterator(*this);
}

PathIterator Path::end() const
{
	return PathIterator(*this, true);
}
/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/FileSystem/Path.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

//Operators
Path Path::operator/(const Path &rhs) const
{
	ASSERT(!this->pathString.IsEmpty(), u8"this must be non-empty.");
	ASSERT(!rhs.IsAbsolute(), u8"rhs must not be absolute.");

	if(this->pathString.EndsWith(u8"/"))
		return Path(this->pathString + rhs.pathString);
	return Path(this->pathString + u8"/" + rhs.pathString);
}

//Public methods
String Path::GetFileExtension() const
{
	uint32 posSlash = this->pathString.FindReverse(u8"/");

	uint32 posDot;
	if(posSlash == Unsigned<uint32>::Max())
		posDot = this->pathString.FindReverse(u8".");
	else
		posDot = this->pathString.FindReverse(u8".", this->pathString.GetLength()-1, posSlash);

	if(posDot == Unsigned<uint32>::Max())
		return String();

	return this->pathString.SubString(posDot + 1, this->pathString.GetLength() - posDot - 1).ToLowercase();
}

String Path::GetName() const
{
	uint32 pos;

	pos = this->pathString.FindReverse(u8"/");

	if(pos == Unsigned<uint32>::Max())
		return this->pathString;

	return this->pathString.SubString(pos + 1, this->pathString.GetLength() - pos - 1);
}

Path Path::GetParent() const
{
	uint32 pos;

	pos = this->pathString.FindReverse(u8"/");
	if(pos == Unsigned<uint32>::Max())
		return Path();
	if(pos == 0)
		return String(u8"/");

	return Path(this->pathString.SubString(0, pos));
}

String Path::GetTitle() const
{
	uint32 posDot, posSlash;

	posDot = this->pathString.FindReverse(u8".");
	posSlash = this->pathString.FindReverse(u8"/", posDot);

	if(posSlash == Unsigned<uint32>::Max())
	{
		if(posDot == Unsigned<uint32>::Max())
			return this->pathString;
		return this->pathString.SubString(0, posDot);
	}
	if(posDot == Unsigned<uint32>::Max())
		return this->pathString.SubString(posSlash + 1, this->pathString.GetLength() - posSlash - 1);
	if(posSlash > posDot)
		return String();

	return this->pathString.SubString(posSlash + 1, posDot - posSlash - 1);
}

Path Path::Normalized() const
{
	//split into parts first
	Path tmp = *this;
	DynamicArray<String> parts;

	if(this->IsAbsolute())
		parts.Push(u8"/");

	while(!tmp.GetString().IsEmpty())
	{
		String part = tmp.SplitOutmostPathPart(tmp);

		//empty string is caused by double slashes
		if(part.IsEmpty())
			continue;

		//skip current directory
		if(part == u8".")
			continue;

		//resolve parent directory
		if(part == u8"..")
		{
			ASSERT(!parts.IsEmpty(), u8"REPORT THIS PLEASE!");
			parts.Pop();
			continue;
		}

		parts.Push(part);
	}

	//reconstruct path from parts
	Path result = parts[0];
	for(uint32 i = 1; i < parts.GetNumberOfElements(); i++)
	{
		result /= parts[i];
	}
	return result;
}

Path Path::RelativePath(const Path &relativeTo) const
{
	ASSERT(this->IsAbsolute() && relativeTo.IsAbsolute(), u8"How can this work if both are not absolute? We have no knowledge of any file system.")
	Path self = *this;
	Path relative = relativeTo;
	while(!relative.GetString().IsEmpty())
	{
		String choppedSelf = self.SplitOutmostPathPart(self);
		String choppedRel = relative.SplitOutmostPathPart(relative);
		ASSERT(choppedRel == choppedSelf, u8"this correlates with the above assert");
	}
	return self;
}

String Path::SplitOutmostPathPart(Path &subPath) const
{
	uint32 posSlash = this->pathString.Find(u8"/");
	if(posSlash == Unsigned<uint32>::Max())
	{
		String outmost = this->pathString; //do this first so that, in case subPath is this
		subPath = u8"";
		return outmost;
	}
	String outmost = this->pathString.SubString(0, posSlash); //again handle case where subpath is this
	subPath = this->pathString.SubString(posSlash+1);
	return outmost;
}
/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/Filesystem/DirectoryIterator.hpp>
//Namespaces
using namespace StdXX;

//Public methods
String Path::GetFileExtension() const
{
	uint32 posSlash = this->pathString.FindReverse(u8"/");

	uint32 posDot;
	if(posSlash == Unsigned<uint32>::Max())
		posDot = this->pathString.FindReverse(u8".");
	else
		posDot = this->pathString.Find(u8".", posSlash);

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
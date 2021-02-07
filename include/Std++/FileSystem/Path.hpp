/*
 * Copyright (c) 2017-2021 Amir Czwink (amir130@hotmail.de)
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
#pragma once
//Local
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Streams/Writers/TextWriter.hpp>

namespace StdXX::FileSystem
{
	/**
	 * Represents a path to a file or directory.
	 * This class has very much in common with URLs, except that it also supports relative paths.
	 * It does not know any context in which it is used.
	 * For instance it doesn't know if the path is used on the local file system or somewhere else.
	 * As separator, this class always uses '/' no matter which underlying file system or operating system is used.
	 * There is always exactly one root path, which is "/".
	 * For instance the path "C:\" on Windows is represented as "/C:/" by this class.
	 * Paths pointing to files are called "file paths" and paths pointing to directories are called "directory paths".
	 *
	 * To understand the anatomy of a path, lets look at an example:
	 *
	 * Then the "root" path is: "C:/"
	 * The "title" of the path is: "blub"
	 * and the "extension" is: "txt"
	 * -----------------------------
	 * Other examples:
	 *
	 * For path "/bla/bli/blub"
	 *
	 * root: /
	 * title: "blub"
	 * extension: ""
	 *
	 * TODO: example for a relative path
	 */
	class Path
	{
	public:
		//Constructors
		inline Path()
		{
		}

		inline Path(const String& pathString)
		{
			this->pathString = pathString;
		}

		//Properties
		inline const class String& String() const
		{
			return this->pathString;
		}

		//Operators
		inline Path &operator=(const class String& pathString)
		{
			this->pathString = pathString;

			return *this;
		}

		inline bool operator==(const Path& right) const
		{
			return this->pathString == right.pathString;
		}

		inline bool operator<(const Path& right) const
		{
			return this->pathString < right.pathString;
		}

		inline bool operator>(const Path& right) const
		{
			return this->pathString > right.pathString;
		}

		inline Path operator+(const Path& other) const
		{
			return Path(this->String() + other.String());
		}

		/**
		 * Joins two paths together.
		 * this must be non-empty.
		 * rhs must not be absolute.
		 * @param rhs
		 * @return
		 */
		Path operator/(const Path &rhs) const;

		inline Path &operator/=(const Path &rhs)
		{
			*this = *this / rhs;

			return *this;
		}

		//Methods
		class String GetFileExtension() const;
		/**
		 * Returns the name of the file or directory that the path refers to.
		 * Examples:
		 * Let path be: "/C:/bla/bli/blub.txt"
		 * Then the name is: "blub.txt"
		 *
		 * Let path be "/bla/bli/blub"
		 * Then the name is: "blub"
		 * @return
		 */
		class String GetName() const;
		/**
		 * Returns the parent path of this path.
		 * If this path is a directory path, then the parent directory path is returned.
		 * If this path is a file path, then the directory path containing the file is returned.
		 * Examples:
		 * Let path be: "/C:/bla/bli/blub.txt"
		 * Then "parent" path is: "/C:/bla/bli" (i.e. the directory in this case)
		 *
		 * Let path be "/bla/bli/blub"
		 * Then parent path is: /bla/bli
		 *
		 * Let path be "/bla/bli/blub/"
		 * Then parent path is: /bla/bli
		 *
		 * @return
		 */
		Path GetParent() const;
		class String GetTitle() const;
		Path Normalized() const;
		Path RelativePath(const Path& relativeTo) const;
		/**
		 * Returns the leftmost part of this path and sets the remaining part to the 'subPath' arg.
		 * Example:
		 * Let this be: "bla/bli/blub"
		 * Then this method returns "bla" and sets 'subPath' to "bli/blub".
		 * In case this is absolute, the return value will be the empty string.
		 * @param path
		 * @return
		 */
		class String SplitOutmostPathPart(Path &subPath) const;

		//Inline
		inline bool IsAbsolute() const
		{
			return this->pathString.StartsWith(u8"/");
		}

		inline bool IsParentOf(const Path& path) const
		{
			return path.Normalized().String().StartsWith(this->Normalized().String());
		}

		inline bool IsRelative() const
		{
			return !this->IsAbsolute();
		}

		inline bool IsRoot() const
		{
			return this->pathString.GetLength() == 1 && this->pathString.StartsWith(u8"/");
		}

	private:
		//Members
		class String pathString;
	};

	inline TextWriter &operator<<(TextWriter& textWriter, const Path &path)
	{
		return textWriter << path.String();
	}
}
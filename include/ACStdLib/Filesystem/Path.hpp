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
#pragma once
//Local
#include "../Containers/Strings/String.hpp"
#include "ACStdLib/Time/Time.hpp"

namespace ACStdLib
{
    //Forward declarations
    class PathIterator;

	/**
	 * Represents a path on the local file system.
	 * As separator, this class always uses '/' no matter which underlying file system or operating system is used.
	 * Path pointing to files are called "file paths" and paths pointing to directories are called "directory paths".
	 *
	 * To understand the anatomy of a path, lets look at an example:
	 * Let path be: "C:/bla/bli/blub.txt"
	 *
	 * Then the "root" path is: "C:/"
	 * The "parent" path is: "C:/bla/bli" (i.e. the directory in this case)
	 * The "name" of the path is: "blub.txt"
	 * The "title" of the path is: "blub"
	 * and the "extension" is: "txt"
	 * -----------------------------
	 * Other examples:
	 *
	 * For path "/bla/bli/blub"
	 *
	 * root: /
	 * parent: /bla/bli
	 * name: "blub"
	 * title: "blub"
	 * extension: ""
	 *
	 * TODO: example for a relative path
	 */
    class ACSTDLIB_API Path
    {
    public:
        //Constructors
        inline Path()
        {
        }

        inline Path(const String &refPathString)
        {
            this->pathString = refPathString;
        }

        //Operators
        inline Path &operator=(const String &refPathString)
        {
            this->pathString = refPathString;

            return *this;
        }

        inline bool operator==(const Path &refRight) const
        {
            return this->pathString == refRight.pathString;
        }

        inline bool operator<(const Path &refRight) const
        {
            return this->pathString < refRight.pathString;
        }

        inline bool operator>(const Path &refRight) const
        {
            return this->pathString > refRight.pathString;
        }

        inline Path operator+(const Path &other)
		{
			return Path(this->GetString() + other.GetString());
		}

        inline Path operator/(const Path &refRight) const
        {
            return Path(this->pathString + u8"/" + refRight.pathString);
        }

        inline Path &operator/=(const Path &refRight)
        {
            this->pathString += u8"/" + refRight.pathString;

            return *this;
        }

        //Methods
        bool CreateDirectory();
        bool CreateDirectoryTree();
        bool Exists() const;
		Path GetAbsolutePath() const;
        String GetFileExtension() const;
        Time GetLastModifiedTime() const;
        String GetName() const;
        Path GetParent() const;
        Path GetRoot() const;
        String GetTitle() const;
        bool IsDirectory() const;

        //Inline
        inline bool IsRoot() const
		{
			return *this == this->GetRoot();
		}

        inline const String &GetString() const
        {
            return this->pathString;
        }

        //For range-based loop
        PathIterator begin() const;
        PathIterator end() const;

	private:
		//Members
		String pathString;
    };
}
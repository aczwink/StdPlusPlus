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
#include "ACStdLib/Containers/Strings/String.hpp"
#include "ACStdLib/Time/Time.hpp"

/*
Naming conventions:
	For path "C:/bla/bli/blub.txt"

	root path: "C:/"
	parent path: "C:/bla/bli"
	name: "blub.txt"
	title: "blub"
	extension: "txt"

	-----------------------------
	For path "C:/bla/bli/blub"

	root path: C:/
	parent path: C:/bla/bli
	name: "blub"
	title: "blub"
	extension: ""

	-----------------------------
	For path "../blub"

	root path: ????????????????????????????UNDEFINED??????????????????????????
	parent path: ????????????????????????????UNDEFINED??????????????????????????
	name: "blub"
*/
namespace ACStdLib
{
    //Forward declarations
    class PathIterator;

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
            return Path(this->pathString + '/' + refRight.pathString);
        }

        inline Path &operator/=(const Path &refRight)
        {
            this->pathString += '/' + refRight.pathString;

            return *this;
        }

        //Methods
        bool CreateDirectory();
        bool CreateDirectoryTree();
        bool Exists() const;
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
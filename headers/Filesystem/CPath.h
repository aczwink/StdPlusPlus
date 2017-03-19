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
#include "../Containers/Strings/CString.h"
#include "../Time/CTime.h"

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
    class CPathIterator;

    class ACSTDLIB_API CPath
    {
    private:
        //Members
        CString pathString;
    public:
        //Constructors
        inline CPath()
        {
        }

        inline CPath(const CString &refPathString)
        {
            this->pathString = refPathString;
        }

        //Operators
        inline CPath &operator=(const CString &refPathString)
        {
            this->pathString = refPathString;

            return *this;
        }

        inline bool operator==(const CPath &refRight) const
        {
            return this->pathString == refRight.pathString;
        }

        inline bool operator<(const CPath &refRight) const
        {
            return this->pathString < refRight.pathString;
        }

        inline bool operator>(const CPath &refRight) const
        {
            return this->pathString > refRight.pathString;
        }

        inline CPath operator/(const CPath &refRight) const
        {
            return CPath(this->pathString + '/' + refRight.pathString);
        }

        inline CPath &operator/=(const CPath &refRight)
        {
            this->pathString += '/' + refRight.pathString;

            return *this;
        }

        //Methods
        bool CreateDirectory();
        bool CreateDirectoryTree();
        bool Exists() const;
        CString GetFileExtension() const;
        CTime GetLastModifiedTime() const;
        CString GetName() const;
        CPath GetParent() const;
        CPath GetRoot() const;
        CString GetTitle() const;
        bool IsDirectory() const;

        //Inline
        inline const CString &GetString() const
        {
            return this->pathString;
        }

        //For range-based loop
        CPathIterator begin() const;
        CPathIterator end() const;
    };

    //Binary operators
    CPath ACSTDLIB_API operator+(const CPath &refLeft, const CPath &refRight);
}
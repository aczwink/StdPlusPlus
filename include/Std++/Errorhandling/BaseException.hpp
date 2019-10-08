/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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

namespace StdXX
{
	//Forward declarations
	//class String;

	class BaseException
	{
	public:
		//Constructors
		inline BaseException() : lineNumber(Unsigned<uint32>::Max())
		{
		}

		inline BaseException(const String& fileName, uint32 lineNumber, const String& functionName) :
			fileName(fileName), lineNumber(lineNumber), functionName(functionName)
		{
		}

		//Abstract
		virtual String Description() const = 0;

		//Inline
		inline String ToString() const
		{
			return this->Description() + this->SourceInfoToString();
		}

	private:
		//Members
		String fileName;
		uint32 lineNumber;
		String functionName;

		//Inline
		inline bool HaveSourceInfo() const
		{
			return this->lineNumber != Unsigned<uint32>::Max();
		}

		inline String SourceInfoToString() const
		{
			if(!this->HaveSourceInfo())
				return {};
			return u8"\nFile: " + this->fileName + u8" (" + String::Number(lineNumber)
			+ u8")\nFunction: " + this->functionName;
		}
	};
}
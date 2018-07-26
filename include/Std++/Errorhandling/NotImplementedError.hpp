/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include "Error.hpp"
#include <Std++/Containers/Strings/String.hpp>

namespace StdXX
{
	class STDPLUSPLUS_API NotImplementedError : public Error
	{
	public:
		//Constructor
		inline NotImplementedError(const char *fileName, uint32 lineNumber, const char *functionName)
				: fileName(fileName), lineNumber(lineNumber), functionName(functionName)
		{
		}

		//Inline
		inline String GetDescription() const override
		{
			return String(u8"You've reached a point in the program that is not implemented.")
				   + u8"\nFile: " + this->fileName + u8" (" + String::Number(lineNumber)
				   + u8")\nFunction: " + this->functionName;
		}

	private:
		const char *fileName;
		uint32 lineNumber;
		const char *functionName;
	};
}
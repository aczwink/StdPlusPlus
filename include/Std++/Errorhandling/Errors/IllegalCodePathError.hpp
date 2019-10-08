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
#include <Std++/Errorhandling/Error.hpp>

namespace StdXX::ErrorHandling
{
	/**
	 * This error indicates that a program reached a state in the code, that is unreachable under normal circumstances.
	 *
	 * The textbook example is a switch-case on an enum.
	 * Even if every case of the enum is handled, the default case can happen at runtime.
	 * For instance when the enum is extended, there is a new case that is not handled in the switch.
	 * This can be even worse when the enum is in a shared library and the library gets updated,
	 * while the application was not recompiled.
	 * In such a case it is recommended to define the default case and throw the IllegalCodePathError, NOT the
	 * NotImplementedError, which may seem appropriate but indicates a to-do.
	 *
	 */
	class IllegalCodePathError : public Error
	{
	public:
		//Constructor
		inline IllegalCodePathError(const String& fileName, uint32 lineNumber, const String& functionName) :
				Error(fileName, lineNumber, functionName)
		{
		}

		//Properties
		String Description() const override
		{
			return u8"You've reached an illegal point in the program.";
		}
	};
}
/*
 * Copyright (c) 2018-2019 Amir Czwink (amir130@hotmail.de)
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
#include "Std++/Errorhandling/Error.hpp"
#include <Std++/Containers/Strings/String.hpp>

namespace StdXX::ErrorHandling
{
	class STDPLUSPLUS_API AssertionError : public Error
	{
	public:
		//Constructor
		inline AssertionError(const String &expression, const String &message, const String& fileName, uint32 lineNumber, const String& functionName) :
			Error(fileName, lineNumber, functionName),
			expression(expression), message(message)
		{
		}

		//Inline
		inline String Description() const override
		{
			return u8"Expression: " + this->expression + u8"\n" + this->message;
		}

	private:
		String expression;
		String message;
	};
}
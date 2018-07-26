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
#include <Std++/Definitions.h>

namespace StdXX
{
	//Forward declarations
	class String;

	/**
	 * An error describes a fatal fault in the program, which is usually irrecoverable.
	 * An example would be for instance when the process is out of memory or an assertion fails.
	 * In such cases, instances of (subclasses of) Error are thrown.
	 * It is not intended that errors are catched.
	 */
	class STDPLUSPLUS_API Error
	{
	public:
		//Abstract
		virtual String GetDescription() const = 0;
	};
}
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
#include <Std++/Streams/Writers/StdOut.hpp>

namespace StdXX
{
	class DebugOut
	{
	public:
		//Inline
		inline DebugOut &operator<<(float64 f)
		{
#ifdef XPC_BUILDTYPE_DEBUG
			stdOut << f;
#endif
			return *this;
		}

		inline DebugOut &operator<<(const char* string)
		{
#ifdef XPC_BUILDTYPE_DEBUG
			stdOut << string;
#endif
			return *this;
		}

		inline DebugOut &operator<<(const LineBreak &refLineBreak)
		{
#ifdef XPC_BUILDTYPE_DEBUG
			stdOut << endl;
#endif
			return *this;
		}
	};

	//Global instances
	extern DebugOut STDPLUSPLUS_API dbgOut;
}
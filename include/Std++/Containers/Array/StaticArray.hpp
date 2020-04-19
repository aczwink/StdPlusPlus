/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
	/**
	 * A static array is an array with a constant size (i.e. its size is static and is known at compile time).
	 * @tparam DataType
	 * @tparam N
	 */
	template <typename DataType, uint32 N>
	class StaticArray
	{
	public:
		//Members
		DataType elements[N];

		//Range-based loop
		constexpr const DataType* begin() const
		{
			return this->elements;
		}

		constexpr const DataType* end() const
		{
			return this->elements + N;
		}
	};
}
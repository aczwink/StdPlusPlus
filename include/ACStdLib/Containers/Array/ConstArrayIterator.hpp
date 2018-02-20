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

namespace ACStdLib
{
	//This class is unfortunately needed because of the need of "array" to be const.
	//template <template <typename DataType> typename ArrayType, typename DataType>
	template<typename ArrayType, typename DataType>
	class ConstArrayIterator
	{
		friend ArrayType;
	public:
		//Operators
		ConstArrayIterator &operator++() //Prefix ++
		{
			this->index++;

			return *this;
		}

		const DataType &operator*() const
		{
			return this->array[this->index];
		}

		bool operator!=(const ConstArrayIterator &refOther) const
		{
			return (&this->array != &refOther.array) || (this->index != refOther.index);
		}

		//Methods
		inline uint32 GetIndex() const
		{
			return this->index;
		}

	private:
		//Members
		const ArrayType &array;
		uint32 index;

		ConstArrayIterator(const ArrayType &array, uint32 index = 0) : array(array)
		{
			this->index = index;
		}
	};
}
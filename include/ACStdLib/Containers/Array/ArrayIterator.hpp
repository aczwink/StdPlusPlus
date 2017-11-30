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

namespace ACStdLib
{
	//template <typename DataType, template <typename> typename ArrayType>
	template <template <typename DataType> typename ArrayType, typename DataType>
    class ArrayIterator
    {
        friend class ArrayType<DataType>;
    public:
        //Operators
        ArrayIterator &operator++() //Prefix ++
        {
            this->index++;

            return *this;
        }

        DataType &operator*()
        {
            return this->array[this->index];
        }

		const DataType &operator*() const
		{
			return this->array[this->index];
		}

        bool operator!=(const ArrayIterator &refOther) const
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
		ArrayType<DataType> &array;
		uint32 index;

		//Constructors
		ArrayIterator(ArrayType<DataType> &refArray, uint32 index = 0) : array(refArray)
		{
			this->index = index;
		}
    };
}
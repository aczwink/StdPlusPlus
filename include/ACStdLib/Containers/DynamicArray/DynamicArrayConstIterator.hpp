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
    //Forward Declarations
    template<typename DataType>
    class DynamicArray;

    template<typename DataType>
    class DynamicArrayConstIterator
    {
        friend class DynamicArray<DataType>;
    private:
        //Members
        const DynamicArray<DataType> &refArray;
        uint32 index;

        //Constructor
        DynamicArrayConstIterator(const DynamicArray<DataType> &refArray, uint32 index = 0) : refArray(refArray)
        {
            this->index = index;
        }
    public:
        //Operators
        DynamicArrayConstIterator &operator++() //Prefix ++
        {
            this->index++;

            return *this;
        }

        const DataType &operator*() const
        {
            return this->refArray[this->index];
        }

        bool operator!=(const DynamicArrayConstIterator &refOther) const
        {
            return (&this->refArray != &refOther.refArray) || (this->index != refOther.index);
        }

        //Functions
        inline uint32 GetIndex() const
        {
            return this->index;
        }
    };
}
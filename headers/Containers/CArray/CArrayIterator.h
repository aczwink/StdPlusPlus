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
    class CArray;

    template<typename DataType>
    class CArrayIterator
    {
        friend class CArray<DataType>;
    private:
        //Members
        CArray<DataType> &refArray;
        uint32 index;

        //Constructor
        CArrayIterator(CArray<DataType> &refArray, uint32 index = 0) : refArray(refArray)
        {
            this->index = index;
        }
    public:
        //Operators
        CArrayIterator &operator++() //Prefix ++
        {
            this->index++;

            return *this;
        }

        DataType &operator*()
        {
            return this->refArray[this->index];
        }

        bool operator!=(const CArrayIterator &refOther)
        {
            return (&this->refArray != &refOther.refArray) || (this->index != refOther.index);
        }

        //Methods
        inline uint32 GetIndex() const
        {
            return this->index;
        }
    };
}
/*
 * Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/Debug.hpp>
#include <Std++/Math/Range.hpp>
#include "LinkedList/LinkedList.hpp"

namespace StdXX
{
    template<typename DataType>
    class OrderedRangeSet
    {
    public:
        //Inline operators
        inline bool operator<(const OrderedRangeSet<DataType>& rhs) const
        {
            return this->ranges.Last() < rhs.ranges.Last();
        }

        inline bool operator>(const OrderedRangeSet<DataType>& rhs) const
        {
            return rhs < *this;
        }

        //Methods
        void Insert(Math::Range<DataType>&& range)
        {
            auto it = this->ranges.begin();
            while((it != this->ranges.end()) && (*it < range))
                ++it;

            this->ranges.InsertBefore(it.GetIndex(), Move(range));
        }

        //Inline
        inline bool IsEmpty() const
        {
            return this->ranges.IsEmpty();
        }

    private:
        //Members
        LinkedList<Math::Range<uint32>> ranges;
    };
}
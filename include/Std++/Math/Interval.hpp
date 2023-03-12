/*
 * Copyright (c) 2019-2023 Amir Czwink (amir130@hotmail.de)
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

#include <Std++/Mathematics.hpp>

namespace StdXX::Math
{
    /**
     * A closed interval between two points i.e. the start and endpoint are included in the range.
     */
    template<typename DataType>
    class Interval
    {
    public:
        //Members
        DataType start;
        DataType end;

        //Constructors
        inline Interval(DataType&& start, DataType&& end)
            : start(Move(start)), end(Move(end))
        {
        }

        inline Interval(const DataType& start, const DataType& end)
                : start(start), end(end)
        {
        }

        //Operators
        inline bool operator==(const Interval<DataType>& rhs) const
        {
            return (this->start == rhs.start) && (this->end == rhs.end);
        }

        inline bool operator<(const Interval<DataType>& rhs) const
        {
            return this->end < rhs.start;
        }

        //Inline
        inline bool Contains(DataType value) const
		{
			return Math::IsValueInInterval(value, this->start, this->end);
		}

		inline bool IsSubsetOf(const Interval<DataType>& other) const
        {
            return (this->start >= other.start) and (this->end <= other.end);
        }

        /**
         * True, if the intersection of this range with the argument is not empty.
         */
        inline bool Overlaps(const Interval<DataType>& other) const
		{
            return other.Contains(this->start) or other.Contains(this->end) or other.IsSubsetOf(*this);
		}
    };
}
/*
 * Copyright (c) 2019-2021 Amir Czwink (amir130@hotmail.de)
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
    template<typename DataType>
    class Range
    {
    public:
        //Members
        DataType start;
        DataType end;

        //Constructors
        inline Range(DataType&& start, DataType&& end)
            : start(Move(start)), end(Move(end))
        {
        }

        inline Range(const DataType& start, const DataType& end)
                : start(start), end(end)
        {
        }

        //Operators
        inline bool operator==(const Range<DataType>& rhs) const
        {
            return (this->start == rhs.start) && (this->end == rhs.end);
        }

        inline bool operator<(const Range<DataType>& rhs) const
        {
            return this->end < rhs.start;
        }

        //Inline
        inline bool ContainsEndInclusive(DataType value) const
		{
			return Math::IsValueInInterval(value, this->start, this->end);
		}

        inline bool Overlaps(const Range<DataType>& other) const
		{
        	return Math::IsValueBetween(this->start, other.start, other.end)
        	or
        	Math::IsValueBetween(this->end, other.start, other.end);
		}
    };
}
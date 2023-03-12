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
//Local
#include <Std++/Debug.hpp>
#include <Std++/Math/Interval.hpp>
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
            if(this->ranges.IsEmpty() and rhs.ranges.IsEmpty())
                return false;
            if(this->ranges.IsEmpty())
                return true;
            return this->ranges.Last() < rhs.ranges.Last();
        }

        inline bool operator>(const OrderedRangeSet<DataType>& rhs) const
        {
            return rhs < *this;
        }

        inline bool operator==(const OrderedRangeSet<DataType>& rhs) const
        {
            return this->ranges == rhs.ranges;
        }

        //Properties
        inline const LinkedList<Math::Interval<DataType>>& Ranges() const
        {
            return this->ranges;
        }

        //Methods
        bool Contains(const DataType& value) const
        {
            for(const auto& range : this->ranges)
            {
                if(range.Contains(value))
                    return true;
            }
            return false;
        }

        void Insert(Math::Interval<DataType>&& range)
        {
            if(this->ranges.IsEmpty())
            {
                this->InsertRange(-1, Move(range));
                return;
            }

            int32 idx = -1;
            for(const auto& refRange : this->ranges)
            {
                if(range.end < refRange.start)
                {
                    //insert before current range
                    this->InsertRange(idx, Move(range));

                    this->Merge(idx+1);
                    return;
                }

                idx++;
            }

            //append at the tail
            this->InsertRange(this->ranges.GetNumberOfElements() - 1, Move(range));
            this->Merge(this->ranges.GetNumberOfElements() - 1);
        }

        bool Intersects(const OrderedRangeSet<DataType>& rhs) const
        {
            for(const auto& range1 : this->ranges)
            {
                for(const auto& range2 : rhs.ranges)
                {
                    if(range1.Overlaps(range2))
                        return true;
                }
            }
            return false;
        }

        void Remove(const DataType& from, const DataType& to)
        {
            int32 idx;

            idx = 0;
            for(auto it = this->ranges.begin(); it != this->ranges.end(); idx++)
            {
                Math::Interval<uint32>& refRange = *it;

                if(refRange.start >= from && refRange.end > to)
                {
                    //shrink range
                    refRange.start = to + 1;

                    this->Merge(idx);

                    ++it;
                }
                else if(refRange.start >= from && refRange.end <= to)
                {
                    //delete range

                    if(idx != 0)
                        --it;

                    this->ranges.Remove(idx);

                    if(idx == 0)
                        it = this->ranges.begin();
                    else
                        ++it;

                    idx--;
                }
                else if(refRange.start < from && refRange.end > to)
                {
                    //range is too large and needs to be split
                    Math::Interval<uint32> newRange(to + 1, refRange.end);
                    this->ranges.InsertAfter(idx, newRange);

                    refRange.end = from - 1;

                    //dont need to look at the others
                    return;
                }
                else if(refRange.end < from)
                {
                    //range is too low
                    ++it;
                }
                else
                {
                    NOT_IMPLEMENTED_ERROR; //TODO: implement me
                }
            }
        }

        //Inline
        inline bool IsEmpty() const
        {
            return this->ranges.IsEmpty();
        }

    private:
        //Members
        LinkedList<Math::Interval<DataType>> ranges;

        //Methods
        //Inline
        inline void InsertRange(int32 idx, Math::Interval<DataType>&& range)
        {
            if(idx == -1)
                this->ranges.InsertFront(Forward<Math::Interval<DataType>>(range));
            else
                this->ranges.InsertAfter(idx, Forward<Math::Interval<DataType>>(range));
        }

        void Merge(uint32 idx)
        {
            bool merged;

            merged = false;

            //merge left
            if(idx > 0)
            {
                Math::Interval<DataType>& refLeft = this->ranges[idx - 1];
                Math::Interval<DataType>& refCurrent = this->ranges[idx];

                if(refCurrent.start == refLeft.end + 1)
                {
                    refLeft.end = refCurrent.end;
                    this->ranges.Remove(idx);
                    merged = true;
                    idx--;
                }
            }

            //merge right
            if(idx < this->ranges.GetNumberOfElements() - 1)
            {
                Math::Interval<DataType>& refCurrent = this->ranges[idx];
                Math::Interval<DataType>& refRight = this->ranges[idx + 1];

                if(refCurrent.end + 1 == refRight.start)
                {
                    refCurrent.end = refRight.end;
                    this->ranges.Remove(idx + 1);
                    merged = true;
                }
            }

            if(merged)
                this->Merge(idx);
        }
    };
}
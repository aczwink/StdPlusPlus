/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include "../Containers/LinkedList/LinkedList.hpp"

namespace StdXX
{
    class CRange
    {
    public:
        //Members
        uint32 low;
        uint32 high;

        //Operators
        inline bool operator==(const CRange &refOther) const
        {
            return this->low == refOther.low && this->high == refOther.high;
        }
    };

    class CCharSet
    {
    private:
        //Members
        LinkedList<CRange> ranges;

        //Methods
        void Merge(uint32 idx);

        //Inline
        inline void InsertRange(int32 idx, uint32 codePointFrom, uint32 codePointTo)
        {
            CRange range;
            range.low = codePointFrom;
            range.high = codePointTo;

            if(idx == -1)
                this->ranges.InsertFront(range);
            else
                this->ranges.InsertAfter(idx, range);
        }
    public:
        //Constructors
        inline CCharSet()
        {
        }

        inline CCharSet(const CCharSet &refOther) //copy ctor
        {
            *this = refOther;
        }

        inline CCharSet(uint32 codePoint)
        {
            this->Insert(codePoint);
        }

        //Inline Operators
        inline bool operator==(const CCharSet &refOther) const
        {
            return this->ranges == refOther.ranges;
        }

        inline CCharSet &operator=(const CCharSet &refOther)
        {
            this->ranges.Release();

            for(const CRange &refRange : refOther.GetRanges())
                this->Insert(refRange.low, refRange.high);

            return *this;
        }

        inline CCharSet &operator+=(const CCharSet &refOther)
        {
            for(const CRange &refRange : refOther.GetRanges())
                this->Insert(refRange.low, refRange.high);

            return *this;
        }

        inline CCharSet &operator-=(const CCharSet &refOther)
        {
            for(const CRange &refRange : refOther.GetRanges())
                this->Remove(refRange.low, refRange.high);

            return *this;
        }

        //Methods
        CCharSet *GetIntersection(const CCharSet &refOther) const;
        void Insert(uint32 codePointFrom, uint32 codePointTo);
        void Negate();
        void Remove(uint32 codePointFrom, uint32 codePointTo);

        //Inline
        inline bool Contains(uint32 codePoint) const
        {
            for(const CRange &refRange : this->ranges)
            {
                if(Math::IsValueInInterval(codePoint, refRange.low, refRange.high))
                    return true;
            }

            return false;
        }

        inline uint32 GetNumberOfRanges() const
        {
            return this->ranges.GetNumberOfElements();
        }

        inline const LinkedList<CRange> &GetRanges() const
        {
            return this->ranges;
        }

        inline void Insert(uint32 codePoint)
        {
            this->Insert(codePoint, codePoint);
        }

        inline bool Intersects(const CCharSet &refOther) const
        {
            bool intersects;
            const CCharSet *pInterSection;

            pInterSection = this->GetIntersection(refOther);
            intersects = !pInterSection->IsEmpty();
            delete pInterSection;

            return intersects;
        }

        inline bool IsEmpty() const
        {
            return this->ranges.IsEmpty();
        }

        inline void Remove(uint32 codePoint)
        {
            this->Remove(codePoint, codePoint);
        }
    };
}
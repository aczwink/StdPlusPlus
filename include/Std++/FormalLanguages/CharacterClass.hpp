/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/OrderedRangeSet.hpp>

namespace StdXX::FormalLanguages
{
    class CharacterClass : public OrderedRangeSet<uint32>
    {
    public:
        //Inline Operators
        inline CharacterClass& operator-=(const CharacterClass& refOther)
        {
            for(const Math::Interval<uint32> &refRange : refOther.Ranges())
                this->Remove(refRange.start, refRange.end);

            return *this;
        }

        //Inline
        inline void Insert(uint32 codePoint)
        {
            this->Insert(codePoint, codePoint);
        }

        inline void Insert(uint32 firstCodePoint, uint32 lastCodePoint)
        {
            OrderedRangeSet<uint32>::Insert(Math::Interval<uint32>(firstCodePoint, lastCodePoint));
        }

        inline CharacterClass Intersect(const CharacterClass& other) const
        {
            CharacterClass intersection;

            for(const auto& range1 : this->Ranges())
            {
                for(const auto& range2 : other.Ranges())
                {
                    if(range1.Overlaps(range2))
                    {
                        uint32 min = Math::Max(range1.start, range2.start);
                        uint32 max = Math::Min(range1.end, range2.end);

                        intersection.Insert(min, max);
                    }
                }
            }

            return intersection;
        }

        CharacterClass Negated()
        {
            CharacterClass result;
            result.Insert(0, UNICODE_MAX);

            for(const auto& range : this->Ranges())
                result.Remove(range.start, range.end);

            return result;
        }
    };
}
/*
    class CCharSet
    {
    public:
        //Constructors
        inline CCharSet()
        {
        }

        inline CCharSet(const CCharSet &refOther) //copy ctor
        {
            *this = refOther;
        }

        //Inline Operators
        inline CCharSet &operator=(const CCharSet &refOther)
        {
            this->ranges.Release();

            NOT_IMPLEMENTED_ERROR;
            //for(const Interval<uint32> &refRange : refOther.GetRanges())
                //this->Insert(refRange.start, refRange.end);

            return *this;
        }

        inline CCharSet &operator+=(const CCharSet &refOther)
        {
            NOT_IMPLEMENTED_ERROR;
            //for(const Interval<uint32> &refRange : refOther.GetRanges())
                //this->Insert(refRange.start, refRange.end);

            return *this;
        }

        //Methods
        CCharSet *GetIntersection(const CCharSet &refOther) const;

        //Inline
        inline uint32 GetNumberOfRanges() const
        {
            return this->ranges.GetNumberOfElements();
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

        inline void Remove(uint32 codePoint)
        {
            this->Remove(codePoint, codePoint);
        }
    };*/
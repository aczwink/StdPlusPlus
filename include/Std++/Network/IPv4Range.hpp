/*
 * Copyright (c) 2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/LinkedList/LinkedList.hpp>
#include <Std++/Math/Interval.hpp>
#include "IPv4Address.hpp"
#include "CIDRRange.hpp"

namespace StdXX
{
    class IPv4Range : public Math::Interval<IPv4Address>
    {
    public:
        //Constructor
        inline IPv4Range(IPv4Address&& start, IPv4Address&& end) : Math::Interval<IPv4Address>(Move(start), Move(end))
        {
        }

        //Methods
        LinkedList<CIDRRange> ToMinimalCIDRRanges() const;
    };
}
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
//Class header
#include <Std++/Network/IPv4Range.hpp>
//Namespaces
using namespace StdXX;

//Local functions
static int8 FindMostSignificantDifferentBit(uint32 ip1, uint32 ip2)
{
    for(int8 i = 31; i >= 0; i--)
    {
        uint32 mask = 1_u32 << i;
        if( (ip1 & mask) != (ip2 & mask) )
            return i;
    }
    return -1;
}

//Public methods
LinkedList<CIDRRange> IPv4Range::ToMinimalCIDRRanges() const
{
    LinkedList<CIDRRange> ranges;

    uint32 ip1 = this->start.ToHostOrderUInt32();
    uint32 ip2 = this->end.ToHostOrderUInt32();

    int8 diffBit = FindMostSignificantDifferentBit(ip1, ip2);
    uint32 diffMask = (1_u32 << diffBit) - 1;

    for(int8 i = diffBit + 1_u8; i >= 0; i--)
    {
        uint32 hostMask = (1_u32 << i) - 1;
        uint32 subnetMask = Unsigned<uint32>::Max() & ~hostMask;

        if( ((ip1 & hostMask) == 0) && ((ip2 & hostMask) == hostMask) )
        {
            //whole address space is covered and aligns binarily
            ranges.InsertTail(CIDRRange(IPv4Address::FromHostEndianOrder(ip1 & subnetMask), 32_u8 - i));
            break;
        }
        else if( (ip1 & subnetMask) == (ip2 & subnetMask) )
        {
            //address space is only partially covered
            continue;
        }

        //whole address space is covered
        uint32 netAddress = (ip1 & ~diffMask) | (1 << i);
        ranges.InsertTail(CIDRRange(IPv4Address::FromHostEndianOrder(netAddress), 32_u8 - i));
    }

    return ranges;
}

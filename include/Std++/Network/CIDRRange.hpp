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
#include "IPv4Address.hpp"

namespace StdXX
{
    class CIDRRange
    {
    public:
        //Constructor
        inline CIDRRange(IPv4Address&& ipPart, uint8 rangeSize) : ipPart(Move(ipPart)), rangeSize(rangeSize)
        {
        }

        //Properties
        inline const auto& FirstIP() const
        {
            return this->ipPart;
        }

        inline auto LastIP() const
        {
            uint32 mask = (1_u32 << (32 - this->rangeSize)) - 1;
            return IPv4Address(this->ipPart.ToUInt32() | mask);
        }

        //Inline
        inline String ToString() const
        {
            return this->ipPart.ToString() + u8"/" + String::Number(this->rangeSize);
        }

    private:
        //Variables
        IPv4Address ipPart;
        uint8 rangeSize;
    };
}
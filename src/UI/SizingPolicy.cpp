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
//Class header
#include <ACStdLib/UI/SizingPolicy.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Methods
void SizingPolicy::SetPolicy(SSizingAttributes &refAttributes, Policy policy)
{
    switch(policy)
    {
        case Policy::Fixed:
        {
            refAttributes.grow = false;
            refAttributes.shrink = false;
            refAttributes.expand = false;
        }
            break;
        case Policy::Minimum:
        {
            refAttributes.grow = true;
            refAttributes.shrink = false;
            refAttributes.expand = false;
        }
            break;
        case Policy::Preferred:
        {
            refAttributes.grow = true;
            refAttributes.shrink = true;
            refAttributes.expand = false;
        }
            break;
        case Policy::Expanding:
        {
            refAttributes.grow = true;
            refAttributes.shrink = true;
            refAttributes.expand = true;
        }
            break;
    }
}
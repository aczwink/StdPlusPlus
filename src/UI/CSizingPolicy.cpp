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
#include "../../headers/UI/CSizingPolicy.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Methods
void CSizingPolicy::SetPolicy(SSizingAttributes &refAttributes, EPolicy policy)
{
    switch(policy)
    {
        case EPolicy::Fixed:
        {
            refAttributes.grow = false;
            refAttributes.shrink = false;
            refAttributes.expand = false;
        }
            break;
        case EPolicy::Minimum:
        {
            refAttributes.grow = true;
            refAttributes.shrink = false;
            refAttributes.expand = false;
        }
            break;
        case EPolicy::Preferred:
        {
            refAttributes.grow = true;
            refAttributes.shrink = true;
            refAttributes.expand = false;
        }
            break;
        case EPolicy::Expanding:
        {
            refAttributes.grow = true;
            refAttributes.shrink = true;
            refAttributes.expand = true;
        }
            break;
    }
}
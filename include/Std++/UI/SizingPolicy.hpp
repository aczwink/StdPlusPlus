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
#include "../Definitions.h"

namespace StdXX
{
    namespace UI
    {
        class STDPLUSPLUS_API SizingPolicy
        {
        public:
            enum class Policy
            {
                Fixed, //widget wants exactly its size hint
                Minimum, //the size hint is minimal but sufficient. The widget can be expanded but there is no advantage in it
                Preferred, //size hint is optimal. Widget can be shrunk and still be useful. The widget can be expanded but there is no advantage in it
                Expanding, //size hint is sensible but widget can be shrunk and still be useful. The widget can make use of extra space as much as possible
            };

            struct SSizingAttributes
            {
                bool grow; //widget can grow larger than its size hint
                bool shrink; //widget can shrink below its size hint
                bool expand; //widget wants as much space as possible
            };
        private:
            //Members
            SSizingAttributes horzAttributes;
            SSizingAttributes vertAttributes;

            //Methods
            void SetPolicy(SSizingAttributes &refAttributes, Policy policy);

        public:
            //Members
            uint8 horzScale;
            uint8 vertScale;

            //Constructor
            inline SizingPolicy()
            {
                this->horzScale = 1;
                this->vertScale = 1;

                this->SetHorizontalPolicy(Policy::Preferred);
                this->SetVerticalPolicy(Policy::Preferred);
            }

            //Inline
            inline const SSizingAttributes &GetHorizontalAttributes() const
            {
                return this->horzAttributes;
            }

            inline const SSizingAttributes &GetVerticalAttributes() const
            {
                return this->vertAttributes;
            }

            inline void SetHorizontalPolicy(Policy policy)
            {
                this->SetPolicy(this->horzAttributes, policy);
            }

            inline void SetVerticalPolicy(Policy policy)
            {
                this->SetPolicy(this->vertAttributes, policy);
            }
        };
    }
};
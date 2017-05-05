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
#pragma once
//Local
#include "ACStdLib/UI/Widget.h"

namespace ACStdLib
{
    namespace UI
    {
        class ACSTDLIB_API CDropDown : public Widget
        {
            friend class EventQueue;
        private:
            //Members
            CFunction<void()> onSelectionChangedHandler;

        public:
            //Constructor
            CDropDown(WidgetContainer *pParent);

            //Methods
            uint32 AddItem(const String &refText);
            void Clear();
            uint32 GetNumberOfItems() const;
            uint16 GetSelectedIndex() const;
            Size GetSizeHint() const;
            void Select(int32 index);
            void SetHint(const String &refText);

            //Inline
            inline void BindSelectionChanged(const CFunction<void()> &refHandler)
            {
                this->onSelectionChangedHandler = refHandler;
            }
        };
    }
}
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
        class ACSTDLIB_API Slider : public Widget
        {
        private:
            //Methods
            void System_CreateHandle();

        public:
			//Dynamic event handlers
			Function<void()> onValueChangedHandler;

            //Constructor
            Slider(WidgetContainer *pParent);

			//Destructor
			~Slider();

            //Methods
			uint32 GetPosition() const;
            Size GetSizeHint() const;
            void SetMaximum(uint32 max);
            void SetMinimum(uint32 min);
            void SetPosition(uint32 pos);
        };
    }
}
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
#include "ACStdLib/UI/Controls/CheckBox.hpp"
#include "../UI/Controls/CDropDown.h"
#include "ACStdLib/UI/Controls/PushButton.hpp"
#include "../UI/Views/CTreeView.h"
#include "Window.h"

namespace ACStdLib
{
    namespace UI
    {
        class ACSTDLIB_API EventQueue
        {
        protected:
            //Functions
            static inline void DispatchPaintEvent(Widget &refWidget)
            {
                refWidget.OnPaint();
            }

            //Inline functions
            static inline void DispatchCloseEvent(Window &refWnd)
            {
                refWnd.OnClose();
            }

            static inline void DispatchDestroyEvent(Window &refWnd)
            {
                refWnd.onDestroyEventHandler();
            }

            static inline void DispatchResizedEvent(Window &refWnd)
            {
                refWnd.OnResized();
            }

            static inline void DispatchSelectionChangedEvent(CDropDown &refDropDown)
            {
                refDropDown.onSelectionChangedHandler();
            }

            static inline void DispatchSelectionChangedEvent(CTreeView &refTreeView)
            {
                refTreeView.OnSelectionChanged();
            }

            static inline void DispatchActivatedEvent(PushButton &refButton)
            {
                if(refButton.onActivatedHandler)
                    refButton.onActivatedHandler();
            }

            static inline void DispatchToggledEvent(CheckBox &refCheckBox)
            {
                if(refCheckBox.onToggledHandler)
                    refCheckBox.onToggledHandler();
            }

        public:
            //Functions
            static void PostQuitEvent();
            static void ProcessEvents();
            static bool ProcessPendingEvents(int32 &refExitCode);
        };
    }
}
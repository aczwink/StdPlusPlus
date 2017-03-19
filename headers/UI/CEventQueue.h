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
#include "Controls/CCheckBox.h"
#include "Controls/CDropDown.h"
#include "Controls/CPushButton.h"
#include "Views/CTreeView.h"
#include "CWindow.h"

namespace ACStdLib
{
    namespace UI
    {
        class ACSTDLIB_API CEventQueue
        {
        protected:
            //Functions
            static void DispatchPaintEvent(AWidget &refWidget);

            //Inline functions
            static inline void DispatchCloseEvent(CWindow &refWnd)
            {
                refWnd.OnClose();
            }

            static inline void DispatchDestroyEvent(CWindow &refWnd)
            {
                refWnd.onDestroyEventHandler();
            }

            static inline void DispatchResizedEvent(CWindow &refWnd)
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

            static inline void DispatchPushedEvent(CPushButton &refButton)
            {
                if(refButton.onPushedHandler)
                    refButton.onPushedHandler();
            }

            static inline void DispatchToggledEvent(CCheckBox &refCheckBox)
            {
                if(refCheckBox.onToggledHandler)
                    refCheckBox.onToggledHandler();
            }

        public:
            //Functions
            static void PostQuitEvent(int32 exitCode);
            static int32 ProcessEvents();
            static bool ProcessPendingEvents(int32 &refExitCode);
        };
    }
}
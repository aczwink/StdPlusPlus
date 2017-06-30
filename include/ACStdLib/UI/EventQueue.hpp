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
#include "Window.hpp"
#include "Controls/CheckBox.hpp"
#include "Controls/CDropDown.h"
#include "Controls/PushButton.hpp"
#include "Controls/RadioButton.hpp"
#include "Views/CTreeView.h"

namespace ACStdLib
{
    namespace UI
    {
        class ACSTDLIB_API EventQueue
        {
        protected:
            //Inline functions
			static inline void DispatchActivatedEvent(PushButton &refButton)
			{
				if(refButton.onActivatedHandler)
					refButton.onActivatedHandler();
			}

			static inline void DispatchActivatedEvent(RadioButton &button)
			{
				if(button.onActivatedHandler)
					button.onActivatedHandler();
			}

            static inline void DispatchCloseEvent(Window &refWnd)
            {
                refWnd.OnClose();
            }

            static inline void DispatchDestroyEvent(Window &refWnd)
            {
                refWnd.onDestroyEventHandler();
            }

			static inline void DispatchMouseButtonPressed(Widget &widget, MouseButton button, const Point &pos)
			{
				widget.OnMouseButtonPressed(button, pos);
			}

			static inline void DispatchMouseButtonReleased(Widget &widget, MouseButton button, const Point &pos)
			{
				widget.OnMouseButtonReleased(button, pos);
			}

			static inline void DispatchMouseMovedEvent(Widget &widget, const Point &pos)
			{
				widget.OnMouseMoved(pos);
			}

			static inline void DispatchMouseWheelEvent(Widget &widget, int16 delta)
			{
				widget.OnMouseWheelTurned(delta);
			}

            static inline void DispatchPaintEvent(Widget &refWidget)
            {
                refWidget.OnPaint();
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

            static inline void DispatchToggledEvent(CheckBox &refCheckBox)
            {
                if(refCheckBox.onToggledHandler)
                    refCheckBox.onToggledHandler();
            }

        public:
            //Functions
            static void PostQuitEvent();
            static void ProcessEvents();
            static bool ProcessPendingEvents();
        };
    }
}
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
#include "../Time/Clock.hpp"
#include "Views/CTreeView.h"

namespace ACStdLib
{
	//Forward declarations
	class Timer;

    namespace UI
    {
        class ACSTDLIB_API EventQueue
        {
			friend class ACStdLib::Timer;

			struct TimerEntry
			{
				int64 leftTime_usec;
				Timer *timer;
			};

		public:
			//Constructor
			EventQueue();

			//Destructor
			~EventQueue();

			//Methods
			bool EventsPending();
			void PostQuitEvent();
			bool ProcessEvents(bool block = true);

			//Functions
			static EventQueue &GetGlobalQueue();

		private:
			//Members
			void *internal;
			Clock clock; //for timers
			LinkedList<TimerEntry> oneShotTimerQueue;

			//Methods
			void DispatchPendingEvents();
			void DispatchSystemEvents();
			void DispatchTimers();
			void Internal_NotifyTimers();
			uint64 UpdateTimers();
			void WaitForEvents(uint64 minWaitTime_usec);

			//Inline
			inline void AddOneShotTimer(uint64 timeOut_usec, Timer *timer)
			{
				TimerEntry entry;

				entry.leftTime_usec = timeOut_usec;
				entry.timer = timer;

				this->oneShotTimerQueue.InsertTail(entry);
				this->Internal_NotifyTimers();
			}

			inline void RemoveTimer(Timer *timer)
			{
				for(auto it = this->oneShotTimerQueue.begin(); it != this->oneShotTimerQueue.end(); )
				{
					if((*it).timer == timer)
						it.Remove();
					else
						++it;
				}
			}

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
        };
    }
}
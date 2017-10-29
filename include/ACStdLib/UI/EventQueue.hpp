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
#include "../Containers/PriorityQueue.hpp"
#include "../Time/Clock.hpp"
#include "Window.hpp"
#include "Controls/CheckBox.hpp"
#include "ACStdLib/UI/Views/ComboBox.hpp"
#include "Controls/PushButton.hpp"
#include "Controls/RadioButton.hpp"
#include "ACStdLib/UI/Views/TreeView.hpp"

namespace ACStdLib
{
	//Forward declarations
	class Timer;

    namespace UI
    {
        class ACSTDLIB_API EventQueue
        {
			friend class ACStdLib::Timer;

		public:
			//Constructor
			EventQueue();

			//Destructor
			~EventQueue();

			//Methods
			bool EventsPending();
			/**
			 * Dispatches pending events in this event queue and, if \p block is true, waits until new events arrive.
			 *
			 * @param block If \p block is true, the method waits (not busy) for new events. Else the function returns when all pending events were dispatched.
			 * @return false if this event queue was asked to quit and all pending events were dispatched, else true.
			 */
			bool ProcessEvents(bool block = true);

			//Inline
			/**
			 * Indicates that this event queue should be shut down.
			 * All pending events will still be dispatched.
			 */
			inline void Quit()
			{
				this->quit = true;
			}

			//Functions
			static EventQueue &GetGlobalQueue();

		private:
			//Members
			bool quit;
			void *internal;
			PriorityQueue<Timer *, uint64> oneShotTimerQueue;

			//Methods
			void DispatchPendingEvents();
			void DispatchSystemEvents();
			void DispatchTimers();
			uint64 GetShortestTimerTimeOut();
			void NotifyTimers();
			/**
			 * We need these absolute values for implementing timers with priority queue.
			 *
			 * @return Monotonic time value in nanoseconds
			 */
			uint64 QueryMonotonicClock();
			void WaitForEvents(uint64 minWaitTime_usec);

			//Inline
			inline void AddOneShotTimer(uint64 timeOut_usec, Timer *timer)
			{
				this->oneShotTimerQueue.Insert(this->QueryMonotonicClock() + timeOut_usec * 1000, timer);
				this->QueryMonotonicClock();
			}

			inline void RemoveTimer(Timer *timer)
			{
				this->oneShotTimerQueue.Remove(timer);
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

            static inline void DispatchSelectionChangedEvent(View &view)
            {
				view.OnSelectionChanged();
            }

            static inline void DispatchToggledEvent(CheckBox &refCheckBox)
            {
                if(refCheckBox.onToggledHandler)
                    refCheckBox.onToggledHandler();
            }
        };
    }
}
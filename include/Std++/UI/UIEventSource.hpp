/*
* Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/Eventhandling/EventSource.hpp>
#include <Std++/UI/Controls/CheckBox.hpp>
#include <Std++/UI/Views/View.hpp>
#include <Std++/UI/Controls/PushButton.hpp>
#include <Std++/UI/Controls/RadioButton.hpp>
#include "Widget.hpp"
#include "Window.hpp"

namespace StdXX
{
	namespace UI
	{
		class UIEventSource : public EventSource
		{
		protected:
			//Inline
			inline void DispatchActivatedEvent(PushButton &refButton)
			{
				if (refButton.onActivatedHandler.IsBound())
					refButton.onActivatedHandler();
			}

			inline void DispatchActivatedEvent(RadioButton &button)
			{
				if (button.onActivatedHandler.IsBound())
					button.onActivatedHandler();
			}

			inline void DispatchCloseEvent(Window &window)
			{
				window.OnClose();
			}

			inline void DispatchMouseButtonPressed(Widget &widget, const Events::MouseClickEvent &event)
			{
				widget.OnMouseButtonPressed(event);
			}

			inline void DispatchMouseButtonReleased(Widget &widget, const Events::MouseClickEvent &event)
			{
				widget.OnMouseButtonReleased(event);
			}

			inline void DispatchMouseMovedEvent(Widget &widget, const Math::PointD &pos)
			{
				widget.OnMouseMoved(pos);
			}

			inline void DispatchMouseWheelEvent(Widget &widget, int16 delta)
			{
				widget.OnMouseWheelTurned(delta);
			}

			inline void DispatchPaintEvent(Widget &widget)
			{
				widget.OnPaint();
			}

			inline void DispatchSelectionChangedEvent(View &view)
			{
				view.OnSelectionChanged();
			}

			inline void DispatchToggledEvent(CheckBox &checkBox)
			{
				if (checkBox.onToggledHandler.IsBound())
					checkBox.onToggledHandler();
			}
		};
	}
}
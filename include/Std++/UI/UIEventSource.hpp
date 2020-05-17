/*
* Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/EventHandling/EventSource.hpp>
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
		class UIEventSource : public EventHandling::EventSource
		{
		protected:
			inline void DispatchActivatedEvent(RadioButton &button)
			{
				if (button.onActivatedHandler.IsBound())
					button.onActivatedHandler();
			}
		};
	}
}
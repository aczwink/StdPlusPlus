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
#pragma once
//Local
#include <Std++/Containers/LinkedList/LinkedList.hpp>
#include <Std++/UI/Controllers/ControllerIndex.hpp>
#include "Event.hpp"

namespace StdXX
{
	namespace UI
	{
		/**
		 * This event describes that the selection in an View-object changed.
		 * It can describe a difference in selection or an absolute selection.
		 */
		class SelectionChangedEvent : public Event
		{
		public:
			//Constructor
			inline SelectionChangedEvent(LinkedList<ControllerIndex>&& selected) : Event(EventType::SelectionChanged),
				isDiff(false), selected(Move(selected))
			{
			}

			inline SelectionChangedEvent(LinkedList<ControllerIndex>&& selected, LinkedList<ControllerIndex>&& deselected) : Event(EventType::SelectionChanged),
				isDiff(true), selected(Move(selected)), deselected(Move(deselected))
			{
			}

			//Inline
			inline bool DescribesDifference() const
			{
				return this->isDiff;
			}

			inline const LinkedList<ControllerIndex>& Selected() const
			{
				return this->selected;
			}

		private:
			//Members
			bool isDiff;
			LinkedList<ControllerIndex> selected;
			LinkedList<ControllerIndex> deselected;
		};
	}
}
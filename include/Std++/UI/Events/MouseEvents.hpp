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
#include <Std++/UI/Mouse.hpp>
#include <Std++/Math/Point.hpp>
#include "InputEvent.hpp"

namespace StdXX
{
	namespace UI
	{
		class MouseEvent : public InputEvent
		{
		public:
			//Constructor
			inline MouseEvent(EventType eventType, const Math::PointD &pos, const KeyboardModifiers &keyboardModifiers)
				: InputEvent(eventType, keyboardModifiers), pos(pos)
			{
			}

			//Inline
			inline const Math::PointD &GetPosition() const
			{
				return this->pos;
			}

		private:
			//Members
			Math::PointD pos;
		};

		class MouseClickEvent : public MouseEvent
		{
		public:
			//Constructor
			inline MouseClickEvent(MouseButton mouseButton, bool isButtonDown, const Math::PointD &pos, const KeyboardModifiers &keyboardModifiers)
				: MouseEvent(isButtonDown ? EventType::MouseButtonPressed : EventType::MouseButtonReleased, pos, keyboardModifiers), mouseButton(mouseButton)
			{
			}

			//Inline
			inline MouseButton GetMouseButton() const
			{
				return this->mouseButton;
			}

		private:
			//Members
			MouseButton mouseButton;
		};

		class MouseWheelEvent : public InputEvent
		{
		public:
			//Constructor
			inline MouseWheelEvent(int16 dy, const KeyboardModifiers &keyboardModifiers) : InputEvent(EventType::MouseWheelRolled, keyboardModifiers),
				dy(dy)
			{
			}

			//Inline
			inline int16 GetYDelta() const
			{
				return this->dy;
			}

		private:
			//Members
			int16 dy;
		};
	}
}
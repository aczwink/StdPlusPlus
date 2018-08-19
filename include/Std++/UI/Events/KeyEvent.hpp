/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include "InputEvent.hpp"
#include <Std++/UI/Keyboard.hpp>

namespace StdXX
{
    namespace UI
    {
        namespace Events
        {
            class KeyEvent : public InputEvent
            {
            public:
                //Constructor
                inline KeyEvent(KeyCode keyCode, const KeyboardModifiers &keyboardModifiers) : InputEvent(keyboardModifiers)
                {
                    this->keyCode = keyCode;
                }

                //Inline
                inline KeyCode GetKeyCode() const
                {
                    return this->keyCode;
                }

            private:
                //Members
                KeyCode keyCode;
            };
        }
    }
}
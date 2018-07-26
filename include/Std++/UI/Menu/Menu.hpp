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
#include "ActionEntry.hpp"
#include <Std++/_Backends/UI/MenuBackend.hpp>
#include "../../Containers/Array/DynamicArray.hpp"
#include "../../Containers/Strings/OldString.hpp"

namespace StdXX
{
    namespace UI
    {

        class STDPLUSPLUS_API Menu : public MenuEntry
        {
            friend class MenuBar;
		public:
			//Constructor
			Menu(const String &text);

			//Destructor
			~Menu();

			//Inline
			inline void AppendEntry(Action *action)
			{
				ActionEntry *actionEntry = new ActionEntry(action);
				this->menuEntries.Push(actionEntry);
				this->backend->AppendEntry(*actionEntry);
			}

			inline void AppendSeperator()
			{
				this->menuEntries.Push(nullptr);
				this->backend->AppendSeperator();
			}

			inline void AppendSubMenu(Menu *subMenu)
			{
				this->menuEntries.Push(subMenu);
				this->backend->AppendSubMenu(subMenu);
			}

			inline const _stdxx_::MenuBackend *_GetBackend() const
			{
				return this->backend;
			}

			inline const DynamicArray<MenuEntry *> GetEntries() const
			{
				return this->menuEntries;
			}

			inline String GetText() const
			{
				return this->backend->GetText();
			}

        private:
            //Members
			_stdxx_::MenuBackend *backend;
            DynamicArray<MenuEntry *> menuEntries;
        };
    }
}
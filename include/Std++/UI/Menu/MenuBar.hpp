/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/_Backends/UI/MenuBarBackend.hpp>
#include "../../Definitions.h"
#include "../../Containers/Array/DynamicArray.hpp"

namespace StdXX
{
    namespace UI
    {
        //Forward declarations
        class Menu;
        class Window;

        class STDPLUSPLUS_API MenuBar
        {
            friend class Window;
        public:
            //Constructor
			MenuBar();

            //Destructor
			~MenuBar();

            //Inline
			inline void AppendMenu(Menu *menu)
			{
				this->attachedMenus.Push(menu);
				if(this->backend)
					this->backend->AppendMenu(menu);
			}

		private:
			//Members
			_stdxx_::MenuBarBackend *backend;
			DynamicArray<Menu *> attachedMenus;
        };
    }
}
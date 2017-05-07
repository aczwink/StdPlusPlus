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
#include "../../Definitions.h"
#include "../../Containers/DynamicArray/DynamicArray.hpp"

namespace ACStdLib
{
    namespace UI
    {
        //Forward declarations
        class CMenu;
        class Window;

        class ACSTDLIB_API CMenuBar
        {
            friend class Window;
        private:
            //Members
            void *pOSHandle;
            DynamicArray<CMenu *> attachedMenus;

            //Methods
            void AppendMenuOS(CMenu *pMenu);
			void DestroyMenuOS();

        public:
            //Constructor
            CMenuBar();

            //Destructor
            ~CMenuBar();

            //Methods
            void AppendMenu(CMenu *pMenu);
        };
    }
}
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
#include "TreeController.hpp"

namespace StdPlusPlus
{
    namespace UI
    {
        class STDPLUSPLUS_API ListController : public TreeController
        {
        public:
            //Abstract
            virtual uint32 GetNumberOfItems() const = 0;
            virtual String GetText(uint32 index) const = 0;

			//Methods
			ControllerIndex GetChildIndex(uint32 row, uint32 column, const ControllerIndex &parent) const;
			String GetColumnText(uint32 column) const;
			uint32 GetNumberOfChildren(const ControllerIndex &parent) const;
			uint32 GetNumberOfColumns() const;
			ControllerIndex GetParentIndex(const ControllerIndex &index) const;
			String GetText(const ControllerIndex &index) const;
        };
    }
}
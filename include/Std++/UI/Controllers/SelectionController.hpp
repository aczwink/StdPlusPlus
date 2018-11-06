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
#include <Std++/Containers/LinkedList/LinkedList.hpp>
#include <Std++/Definitions.h>
#include "ControllerIndex.hpp"

namespace StdXX
{
	namespace UI
	{
		class SelectionController
		{
		public:
			//Inline
			inline void ClearSelection()
			{
				this->selectedIndexes.Release();
			}

			inline const LinkedList<ControllerIndex> &GetSelectedIndexes() const
			{
				return this->selectedIndexes;
			}

			inline void Select(const ControllerIndex &index)
			{
				this->selectedIndexes.InsertTail(index);
			}

			inline void SetSelection(const LinkedList<ControllerIndex>& selection)
			{
				this->selectedIndexes = selection;
			}

		private:
			//Members
			LinkedList<ControllerIndex> selectedIndexes;
		};
	}
}
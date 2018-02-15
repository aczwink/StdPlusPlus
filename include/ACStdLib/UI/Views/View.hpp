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
#include "../Widget.hpp"
#include "../Controllers/SelectionController.hpp"

namespace ACStdLib
{
	namespace UI
	{
		//Forward declarations
		class TreeController;

		class ACSTDLIB_API View : public Widget
		{
			friend class TreeController;
			friend class EventQueue;
		public:
			//Constructor
			inline View(WidgetContainer *parent) : Widget(parent)
			{
				this->controller = nullptr;
			}

			//Methods
			void SetController(TreeController &controller);

			//Inline
			inline TreeController *GetController()
			{
				return this->controller;
			}

			inline const TreeController *GetController() const
			{
				return this->controller;
			}

			inline const SelectionController &GetSelectionController() const
			{
				return this->selectionController;
			}

		protected:
			//Members
			TreeController *controller;
			SelectionController selectionController;

			//Overrideable event handlers
			virtual void OnSelectionChanged();

		private:
			//Abstract event handlers
			virtual void OnModelChanged() = 0;
		};
	}
}
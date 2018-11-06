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
#include <Std++/_Backends/UI/ViewBackend.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#include <Std++/UI/Controllers/SelectionController.hpp>
#include <Std++/UI/Events/SelectionChangedEvent.hpp>
#include <Std++/UI/Widget.hpp>

namespace StdXX
{
	//Forward declarations
	class EventQueue;

	namespace UI
	{
		//Forward declarations
		class TreeController;

		class STDPLUSPLUS_API View : public Widget
		{
			friend class TreeController;
			friend class UIEventSource;
		public:
			//Constructor
			inline View() : viewBackend(nullptr)
			{
			}

			//Methods
			void Select(const ControllerIndex& index);
			void SetController(UniquePointer<TreeController>&& controller);

			//Overrideable
			virtual void Event(UI::Event& event) override;

			//Inline
			inline TreeController *GetController()
			{
				return this->controller.operator->();
			}

			inline const TreeController *GetController() const
			{
				return this->controller.operator->();
			}

			inline const SelectionController &GetSelectionController() const
			{
				return this->selectionController;
			}

		protected:
			//Members
			UniquePointer<TreeController> controller;

			//Event handlers
			virtual void OnRealized() override;

			//Inline
			inline void _SetBackend(_stdxx_::ViewBackend* viewBackend)
			{
				Widget::_SetBackend(viewBackend);
				this->viewBackend = viewBackend;
			}

		private:
			//Members
			_stdxx_::ViewBackend* viewBackend;
			SelectionController selectionController;

			//Event handlers
			void OnSelectionChanged(SelectionChangedEvent& event);

			//Inline
			inline void OnModelChanged()
			{
				if(this->viewBackend)
					this->viewBackend->ControllerChanged();
			}
		};
	}
}
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
#include <Std++/SmartPointers/UniquePointer.hpp>
#include <Std++/_Backends/UI/ContentAreaWidgetBackend.hpp>
#include "Std++/UI/Containers/CompositeWidget.hpp"
#include "Std++/UI/WidgetContainer.hpp"

namespace StdXX
{
	namespace UI
	{
		/*
		 * A widget with only one child, the content area.
		 * The content area is a container, to which other widgets can be added.
		 */
		class STDPLUSPLUS_API ContentAreaWidget : public WidgetContainer
		{
		public:
			//Constructor
			inline ContentAreaWidget() : contentContainer(nullptr)
			{
			}

			//Destructor
			~ContentAreaWidget();

			//Overrideable
			virtual Math::SizeD GetSizeHint() const;

			//Methods
			Widget *GetChild(uint32 index);
			uint32 GetNumberOfChildren() const;
			void RemoveChild(Widget *child);

			//Inline
			inline void AddContentChild(Widget *widget)
			{
				this->contentContainer->AddChild(widget);
			}

			inline CompositeWidget *GetContentContainer()
			{
				return this->contentContainer;
			}

			inline const CompositeWidget *GetContentContainer() const
			{
				return this->contentContainer;
			}

			inline void SetContentContainer(CompositeWidget *contentContainer)
			{
				this->contentContainer = contentContainer;
				this->TakeWidgetOwnership(contentContainer);
			}

		protected:
			//Members
			_stdxx_::ContentAreaWidgetBackend *contentAreaWidgetBackend;

			//Eventhandlers
			virtual void OnResized();

		private:
			//Members
			CompositeWidget *contentContainer;
		};
	}
}
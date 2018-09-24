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
#include <Std++/_Backends/UI/WidgetContainerBackend.hpp>
#include "Widget.hpp"

namespace StdXX
{
	namespace UI
	{
		class STDPLUSPLUS_API WidgetContainer : public Widget
		{
		public:
			//Constructor
			inline WidgetContainer() : widgetContainerBackend(nullptr)
			{
			}

			//Abstract
			virtual Widget *GetChild(uint32 index) = 0;
			virtual const Widget* GetChild(uint32 index) const = 0;
			virtual uint32 GetNumberOfChildren() const = 0;
			virtual void RemoveChild(Widget *child) = 0;

		protected:
			//Members
			_stdxx_::WidgetContainerBackend *widgetContainerBackend;

			//Eventhandlers
			virtual void OnRealized();

			//Inline
			inline void _SetBackend(_stdxx_::WidgetContainerBackend *widgetContainerBackend)
			{
				Widget::_SetBackend(widgetContainerBackend);
				this->widgetContainerBackend = widgetContainerBackend;
			}

			inline void FreeWidgetOwnership(Widget *widget)
			{
				widget->parent = nullptr;
			}

			inline void TakeWidgetOwnership(Widget *widget)
			{
				if(widget->parent)
					widget->parent->RemoveChild(widget);
				widget->parent = this;
				if (widget->CanRealize())
					widget->Realize();
				if(this->widgetContainerBackend != nullptr)
					this->widgetContainerBackend->AddChild(widget);
			}
		};
	}
}
/*
 * Copyright (c) 2019-2021 Amir Czwink (amir130@hotmail.de)
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
//Class header
#include "Gtk3RedirectContainer.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
Gtk3RedirectContainer::Gtk3RedirectContainer(UIBackend& uiBackend, CompositeWidget &widget) : WidgetContainerBackend(uiBackend),
	Gtk3WidgetBackend(uiBackend, redirect_container_new()), WidgetBackend(uiBackend), widget(widget)
{
}

//Public methods
void Gtk3RedirectContainer::AddChild(Widget *widget)
{
	if(!widget->_IsRealized())
		return;

	Gtk3WidgetBackend* child = dynamic_cast<Gtk3WidgetBackend*>(widget->_GetBackend());
	if(child)
	{
		gtk_container_add(GTK_CONTAINER(this->GetGtkWidget()), child->GetGtkWidget());
	}
	else
	{
		WidgetContainer* widgetContainer = dynamic_cast<WidgetContainer *>(widget);
		for(uint32 i = 0; i < widgetContainer->GetNumberOfChildren(); i++)
			this->AddChild(widgetContainer->GetChild(i));
	}
}

Math::SizeD Gtk3RedirectContainer::GetSizeHint() const
{
	return Math::SizeD();
}

const Widget &Gtk3RedirectContainer::GetWidget() const
{
    return this->widget;
}




//NOT IMPLEMENTED
void _stdxx_::Gtk3RedirectContainer::Repaint() {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
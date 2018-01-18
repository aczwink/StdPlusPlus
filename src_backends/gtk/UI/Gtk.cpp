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
//corresponding header
#include "Gtk.h"
//Local
#include <ACStdLib/UI/WidgetContainer.hpp>

//Global functions
void AddToParent(const Widget *widget, GtkWidget *gtkChild)
{
	widget = widget->GetParent();
	while(widget)
	{
		if(PRIVATE_DATA(widget))
		{
			gtk_container_add(GTK_CONTAINER(PRIVATE_DATA(widget)->childAreaWidget), gtkChild);
			break;
		}
		widget = widget->GetParent();
	}
}

Size GetPreferedSizeGtk(GtkWidget *widget)
{
	int min1, nat1, min2, nat2;

	gtk_widget_get_preferred_width(widget, &min1, &nat1);
	gtk_widget_get_preferred_height(widget, &min2, &nat2);

	return Size(nat1, nat2);
}
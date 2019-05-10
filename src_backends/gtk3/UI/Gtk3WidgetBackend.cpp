/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include "Gtk3WidgetBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Public methods
Math::SizeD Gtk3WidgetBackend::GetSizeHint() const
{
	int min1, nat1, min2, nat2;

	gtk_widget_get_preferred_width(this->gtkWidget, &min1, &nat1);
	gtk_widget_get_preferred_height(this->gtkWidget, &min2, &nat2);

	return Math::SizeD(nat1, nat2);
}

void Gtk3WidgetBackend::IgnoreEvent()
{
	//obsolete method
}

void Gtk3WidgetBackend::SetBounds(const Math::RectD &bounds)
{
	gtk_widget_queue_resize(this->gtkWidget);
}

void Gtk3WidgetBackend::SetEnabled(bool enable)
{
    gtk_widget_set_sensitive(this->gtkWidget, enable);
}

void Gtk3WidgetBackend::SetHint(const String &text)
{
    gtk_widget_set_tooltip_text(this->gtkWidget, reinterpret_cast<const gchar *>(text.ToUTF8().GetRawZeroTerminatedData()));
}

void Gtk3WidgetBackend::Show(bool visible)
{
    if(visible)
        gtk_widget_show(this->gtkWidget);
    else
        gtk_widget_hide(this->gtkWidget);
}
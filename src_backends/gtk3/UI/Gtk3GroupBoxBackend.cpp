/*
 * Copyright (c) 2018-2021 Amir Czwink (amir130@hotmail.de)
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
#include "Gtk3GroupBoxBackend.hpp"
#include "Gtk3RedirectContainer.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
Gtk3GroupBoxBackend::Gtk3GroupBoxBackend(StdXX::UIBackend& uiBackend, GroupBox &groupBox)
    : GroupBoxBackend(uiBackend), WidgetContainerBackend(uiBackend), WidgetBackend(uiBackend),
    Gtk3WidgetBackend(uiBackend, gtk_frame_new(nullptr)),
    groupBox(groupBox)
{
}

//Public methods
void Gtk3GroupBoxBackend::AddChild(Widget *widget)
{
	Gtk3WidgetBackend* backend = dynamic_cast<Gtk3WidgetBackend *>(widget->_GetBackend());
	gtk_container_add(GTK_CONTAINER(this->GetGtkWidget()), backend->GetGtkWidget());
}

WidgetContainerBackend *Gtk3GroupBoxBackend::CreateContentAreaBackend(CompositeWidget &widget)
{
	Gtk3RedirectContainer* container = new Gtk3RedirectContainer(this->GetUIBackend(), widget);
	gtk_container_add(GTK_CONTAINER(this->GetGtkWidget()), container->GetGtkWidget());
    return container;
}

Math::RectD Gtk3GroupBoxBackend::GetContentAreaBounds() const
{
    GtkBin* gtkBin = GTK_BIN(this->GetGtkWidget());
    GtkWidget* gtkChild = gtk_bin_get_child(gtkBin);

    GtkAllocation alloc;
    gtk_widget_get_allocation(gtkChild, &alloc);

    return Math::RectD(alloc.x, alloc.y, alloc.width, alloc.height);
}

const Widget &Gtk3GroupBoxBackend::GetWidget() const
{
    return this->groupBox;
}

void Gtk3GroupBoxBackend::SetTitle(const String &title)
{
    const gchar *gtkText = reinterpret_cast<const gchar *>(title.ToUTF8().GetRawZeroTerminatedData());
    gtk_frame_set_label(GTK_FRAME(this->GetGtkWidget()), gtkText);
}


//NOT IMPLEMENTED
void _stdxx_::Gtk3GroupBoxBackend::Repaint() {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
//END OF NOT IMPLEMENTED
/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
#include "Gtk3TabContainerBackend.hpp"
//Local
#include <Std++/UI/Containers/TabContainer.hpp>
#include "../Gtk3RedirectContainer.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
Gtk3TabContainerBackend::Gtk3TabContainerBackend(UIBackend &uiBackend, TabContainer &tabContainer)
    : TabContainerBackend(uiBackend), WidgetContainerBackend(uiBackend), WidgetBackend(uiBackend), Gtk3WidgetBackend(uiBackend, gtk_notebook_new()),
    tabContainer(tabContainer)
{
}

//Public methods
void Gtk3TabContainerBackend::AddChild(Widget *widget)
{
    Gtk3WidgetBackend* childBackend = dynamic_cast<Gtk3WidgetBackend *>(widget->_GetBackend());
    gtk_notebook_append_page(GTK_NOTEBOOK(this->GetGtkWidget()), childBackend->GetGtkWidget(), gtk_label_new(""));
}

WidgetContainerBackend *Gtk3TabContainerBackend::CreateChildAreaBackend(CompositeWidget& widget)
{
    Gtk3RedirectContainer* container = new Gtk3RedirectContainer(this->GetUIBackend(), widget);
    return container;
}

const Widget& Gtk3TabContainerBackend::GetWidget() const
{
    return this->tabContainer;
}

void Gtk3TabContainerBackend::SetLabel(uint32 tabIndex, const String &label)
{
    GtkWidget* child = gtk_notebook_get_nth_page(GTK_NOTEBOOK(this->GetGtkWidget()), tabIndex);
    gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(this->GetGtkWidget()), child, (const gchar*)label.ToUTF8().GetRawZeroTerminatedData());
}

//NOT IMPLEMENTED
void _stdxx_::Gtk3TabContainerBackend::Repaint() {
    NOT_IMPLEMENTED_ERROR; //TODO: IMPLEMENT ME
}
//NOT IMPLEMENTED
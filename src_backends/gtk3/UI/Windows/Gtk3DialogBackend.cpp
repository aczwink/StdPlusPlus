/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
#include "Gtk3DialogBackend.hpp"
//Local
#include <Std++/UI/Windows/Window.hpp>
#include "../Gtk3WidgetBackend.hpp"
#include "../Gtk3RedirectContainer.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Destructor
Gtk3DialogBackend::~Gtk3DialogBackend()
{
	gtk_widget_destroy(this->GetGtkWidget());
}

//Public methods
void Gtk3DialogBackend::AddChild(StdXX::UI::Widget *widget)
{
	GtkDialog* gtkDialog = GTK_DIALOG(this->GetGtkWidget());
	GtkWidget* contentArea = gtk_dialog_get_content_area(gtkDialog);

	Gtk3WidgetBackend* backend = dynamic_cast<Gtk3WidgetBackend *>(widget->_GetBackend());
	gtk_box_pack_end(GTK_BOX(contentArea), backend->GetGtkWidget(), TRUE, TRUE, 0);
}

bool Gtk3DialogBackend::Run(Property<bool>& isValid)
{
	GtkWindow* gtkWindow = GTK_WINDOW(this->GetGtkWidget());
	GtkDialog* gtkDialog = GTK_DIALOG(this->GetGtkWidget());

	Gtk3WindowBackend* parentWindowBackend = dynamic_cast<Gtk3WindowBackend *>(this->dialog.Parent()->_GetBackend());
	gtk_window_set_transient_for(gtkWindow, GTK_WINDOW(parentWindowBackend->GetGtkWidget()));

	gtk_dialog_add_button(gtkDialog, "Cancel", GTK_RESPONSE_CANCEL);
	gtk_dialog_add_button(gtkDialog, "OK", GTK_RESPONSE_ACCEPT);
	gtk_dialog_set_default_response(gtkDialog, GTK_RESPONSE_ACCEPT);

	isValid.Connect([gtkDialog](bool valid){
		gtk_dialog_set_response_sensitive(gtkDialog, GTK_RESPONSE_ACCEPT, valid ? TRUE : FALSE);
	});
	gtk_dialog_set_response_sensitive(gtkDialog, GTK_RESPONSE_ACCEPT, isValid.Get() ? TRUE : FALSE);

	gint result = gtk_dialog_run(gtkDialog);
	return result == GTK_RESPONSE_ACCEPT;
}
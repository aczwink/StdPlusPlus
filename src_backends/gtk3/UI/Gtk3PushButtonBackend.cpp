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
#include "Gtk3PushButtonBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX::UI;

//Local functions
static void ClickedSlot(GtkButton *button, gpointer user_data)
{
	PushButton& pushButton = *(PushButton *) user_data;
	if(pushButton.onActivatedHandler.IsBound())
		pushButton.onActivatedHandler();
}

//Constructor
Gtk3PushButtonBackend::Gtk3PushButtonBackend(StdXX::UIBackend &uiBackend, PushButton &pushButton)
    : PushButtonBackend(uiBackend), Gtk3WidgetBackend(uiBackend, gtk_button_new()), WidgetBackend(uiBackend),
    pushButton(pushButton)
{
	g_signal_connect(this->GetGtkWidget(), "clicked", G_CALLBACK(ClickedSlot), &pushButton);
}

//Public methods
const Widget &Gtk3PushButtonBackend::GetWidget() const
{
    return this->pushButton;
}

void Gtk3PushButtonBackend::SetText(const StdXX::String &text)
{
    const gchar *gtkText = reinterpret_cast<const gchar *>(text.ToUTF8().GetRawZeroTerminatedData());
    gtk_button_set_label(GTK_BUTTON(this->GetGtkWidget()), gtkText);
}



//NOT IMPLEMENTED
void _stdxx_::Gtk3PushButtonBackend::Repaint() {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
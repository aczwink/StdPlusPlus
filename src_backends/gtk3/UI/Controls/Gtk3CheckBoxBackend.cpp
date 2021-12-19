/*
 * Copyright (c) 2018-2019,2021 Amir Czwink (amir130@hotmail.de)
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
#include "Gtk3CheckBoxBackend.hpp"
//Local
#include <Std++/UI/Events/ValueChangedEvent.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Local functions
static void ToggledSlot(GtkToggleButton* toggleButton, gpointer user_data)
{
    Variant v;
    v.b = gtk_toggle_button_get_active(toggleButton) != 0;
    ValueChangedEvent vce(v);

    CheckBox& checkBox = *(CheckBox*)user_data;
    checkBox.Event(vce);
}

//Constructor
Gtk3CheckBoxBackend::Gtk3CheckBoxBackend(UIBackend &uiBackend, CheckBox &checkBox)
    : CheckBoxBackend(uiBackend), Gtk3WidgetBackend(uiBackend, gtk_check_button_new()), WidgetBackend(uiBackend),
    checkBox(checkBox)
{
    g_signal_connect(this->GetGtkWidget(), "toggled", G_CALLBACK(ToggledSlot), &checkBox);
}

//Public methods
const Widget &Gtk3CheckBoxBackend::GetWidget() const
{
    return this->checkBox;
}

void Gtk3CheckBoxBackend::SetText(const String &text)
{
    gtk_button_set_label(GTK_BUTTON(this->GetGtkWidget()), (const gchar*)text.ToUTF8().GetRawZeroTerminatedData());
}

void Gtk3CheckBoxBackend::UpdateCheckState()
{
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(this->GetGtkWidget()), this->checkBox.Checked());
}

//NOT IMPLEMENTED
void Gtk3CheckBoxBackend::Repaint() {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
//NOT IMPLEMENTED
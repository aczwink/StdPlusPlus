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
#include "Gtk3MenuBackend.hpp"
//Local
#include <Std++/UI/Menu/ActionEntry.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Local functions
static void MenuItemActivate(GtkMenuItem *menuitem, gpointer user_data)
{
    Action* action = static_cast<Action *>(user_data);
    action->triggeredEvent();
}

//Constructor
Gtk3MenuBackend::Gtk3MenuBackend(UIBackend &uiBackend, UI::Menu &menu)
{
    this->gtkWidget = gtk_menu_new();
}

//Public methods
void Gtk3MenuBackend::AppendEntry(const ActionEntry& actionEntry)
{
    GtkWidget* item = gtk_menu_item_new();

    gtk_widget_show(item);
    gtk_menu_item_set_label(GTK_MENU_ITEM(item), (const gchar*)actionEntry.action->text.ToUTF8().GetRawZeroTerminatedData());
    g_signal_connect(item, "activate", G_CALLBACK(MenuItemActivate), actionEntry.action);

    gtk_menu_shell_append(GTK_MENU_SHELL(this->gtkWidget), item);
}

void Gtk3MenuBackend::SetText(const String& text)
{
}

void Gtk3MenuBackend::ShowPopup()
{
    gtk_menu_popup_at_pointer(GTK_MENU(this->gtkWidget), nullptr);
}

//NOT IMPLEMENTED
void Gtk3MenuBackend::AppendSeperator() {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void Gtk3MenuBackend::AppendSubMenu(StdXX::UI::Menu *subMenu) {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
//NOT IMPLEMENTED
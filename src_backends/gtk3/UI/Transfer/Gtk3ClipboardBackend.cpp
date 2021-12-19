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
#include "Gtk3ClipboardBackend.hpp"
//Global
#include <gtk/gtk.h>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Local functions
static void ClipboardGetFunc(GtkClipboard *clipboard, GtkSelectionData *selection_data, guint info, gpointer user_data_or_owner)
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

static void ClipboardClearFunc(GtkClipboard *clipboard, gpointer user_data_or_owner)
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

//Public methods
void Gtk3ClipboardBackend::Store()
{
    GtkClipboard* gtkClipboard = gtk_clipboard_get_default(gdk_display_get_default());

    GtkTargetList* targetList = gtk_target_list_new(nullptr, 0);
    gtk_target_list_add_text_targets(targetList, 0);

    gint nTargets;
    GtkTargetEntry* entries = gtk_target_table_new_from_list(targetList, &nTargets);

    gtk_clipboard_set_with_data(gtkClipboard, entries, nTargets, ClipboardGetFunc, ClipboardClearFunc, nullptr);

    gtk_target_table_free(entries, nTargets);
    gtk_target_list_unref(targetList);
}

void Gtk3ClipboardBackend::Store(const String &text)
{
    GtkClipboard* gtkClipboard = gtk_clipboard_get_default(gdk_display_get_default());

    text.ToUTF8();
    gtk_clipboard_set_text(gtkClipboard, (const gchar*)text.GetRawZeroTerminatedData(), text.GetSize());
}
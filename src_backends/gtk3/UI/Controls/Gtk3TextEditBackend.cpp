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
#include "Gtk3TextEditBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
Gtk3TextEditBackend::Gtk3TextEditBackend(UIBackend &uiBackend, TextEdit &textEdit)
    : TextEditBackend(uiBackend), Gtk3WidgetBackend(uiBackend, gtk_text_view_new()), WidgetBackend(uiBackend),
    textEdit(textEdit)
{
}

//Public methods
const Widget &Gtk3TextEditBackend::GetWidget() const
{
    return this->textEdit;
}

void _stdxx_::Gtk3TextEditBackend::Repaint() {
NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void Gtk3TextEditBackend::SetEditable(bool enable) const
{
    gtk_text_view_set_editable(GTK_TEXT_VIEW(this->GetGtkWidget()), enable);
}

void Gtk3TextEditBackend::SetText(const String &text)
{
    text.ToUTF8();

    GtkTextBuffer* gtkTextBuffer = gtk_text_buffer_new(nullptr);
    gtk_text_buffer_set_text(gtkTextBuffer, (const gchar*)text.GetRawZeroTerminatedData(), text.GetSize());
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(this->GetGtkWidget()), gtkTextBuffer);
}
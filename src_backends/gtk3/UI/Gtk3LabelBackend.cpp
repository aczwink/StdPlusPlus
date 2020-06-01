/*
 * Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
#include "Gtk3LabelBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
Gtk3LabelBackend::Gtk3LabelBackend(UIBackend &uiBackend, Label& label)
	: LabelBackend(uiBackend), WidgetBackend(uiBackend), Gtk3WidgetBackend(uiBackend, gtk_label_new(nullptr)),
	label(label)
{

}

//Public methods
Widget &Gtk3LabelBackend::GetWidget()
{
	return this->label;
}

const Widget &Gtk3LabelBackend::GetWidget() const
{
	return this->label;
}

void Gtk3LabelBackend::SetText(const String &text)
{
	const gchar *gtkText = reinterpret_cast<const gchar *>(text.ToUTF8().GetRawZeroTerminatedData());
	gtk_label_set_text(GTK_LABEL(this->GetGtkWidget()), gtkText);
}


//NOT IMPLEMENTED
void _stdxx_::Gtk3LabelBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::Gtk3LabelBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
//NOT IMPLEMENTED
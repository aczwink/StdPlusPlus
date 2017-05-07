/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include <ACStdLib/UI/Controls/CheckBox.hpp>
//Local
#include "../Gtk.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;
//Definitions
#define THIS (PRIVATE_DATA(this)->widget)

//Destructor
CheckBox::~CheckBox()
{
	MemFree(this->systemHandle);
}

//Private methods
void CheckBox::System_CreateHandle()
{
	this->systemHandle = CreateWidgetPrivateData(gtk_check_button_new(), this);
	gtk_widget_show(THIS); //default to show

	//g_signal_connect(THIS, "clicked", G_CALLBACK(GtkEventQueue::ClickedSlot), this);

	ADD_SELF_TO_PARENT(THIS);
}

//Public methods
Size CheckBox::GetSizeHint() const
{
	return GetPreferedSizeGtk(THIS);
}

String CheckBox::GetText() const
{
	return UTF8String(gtk_button_get_label(GTK_BUTTON(THIS)));
}

bool CheckBox::IsChecked() const
{
	return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(THIS)) != 0;
}

void CheckBox::SetText(const String &text)
{
	UTF8String textUTF8 = text.GetUTF16();

	gtk_button_set_label(GTK_BUTTON(THIS), (const gchar *) textUTF8.GetC_Str());
}
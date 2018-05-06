/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/UI/Controls/CheckBox.hpp>
//Local
#include "../Gtk.h"
#include "../GtkEventSource.hpp"
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;
//Definitions
#define THIS (PRIVATE_DATA(this)->widget)

//Public methods
OldString CheckBox::GetText() const
{
	return UTF8String(gtk_button_get_label(GTK_BUTTON(THIS)));
}

bool CheckBox::IsChecked() const
{
	return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(THIS)) != 0;
}
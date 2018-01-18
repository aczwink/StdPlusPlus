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
#include <ACStdLib/UI/Controls/SpinBox.hpp>
//Local
#include <ACStdLib/Integer.hpp>
#include "../Gtk.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;
//Definitions
#define THIS (PRIVATE_DATA(this)->widget)

/*
 * https://developer.gnome.org/gtk3/stable/GtkSpinButton.html
 */

//Destructor
SpinBox::~SpinBox()
{
	MemFree(this->backend);
}

//Private methods
void SpinBox::System_CreateHandle()
{
	NOT_IMPLEMENTED_ERROR; //TODO: new implementation
	//this->backend = CreateWidgetPrivateData(gtk_spin_button_new(gtk_adjustment_new(0, Integer<int32>::Min(), Integer<int32>::Max(), 1, 5, 5), 1, 0), this);
	gtk_widget_show(THIS); //default to show

	ADD_SELF_TO_PARENT(THIS);
}

//Public methods
Size SpinBox::GetSizeHint() const
{
	return GetPreferedSizeGtk(THIS);
}

int32 SpinBox::GetValue() const
{
	return gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(THIS));
}

void SpinBox::SetMaximum(int32 max)
{
	GtkAdjustment *adjustment = gtk_spin_button_get_adjustment(GTK_SPIN_BUTTON(THIS));

	gtk_spin_button_set_range(GTK_SPIN_BUTTON(THIS), gtk_adjustment_get_lower(adjustment), max);
}

void SpinBox::SetMinimum(int32 min)
{
	GtkAdjustment *adjustment = gtk_spin_button_get_adjustment(GTK_SPIN_BUTTON(THIS));

	gtk_spin_button_set_range(GTK_SPIN_BUTTON(THIS), min, gtk_adjustment_get_upper(adjustment));
}

void SpinBox::SetValue(int32 value)
{
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(THIS), value);
}
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
#include <ACStdLib/UI/Controls/Slider.hpp>
//Local
#include "../Gtk.h"
#include "../GtkEventQueue.hpp"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;
//Definitions
#define THIS (PRIVATE_DATA(this)->widget)

//Local functions
static void ValueChangedSlot(GtkRange *range, gpointer user_data)
{
	Slider *slider = (Slider *) user_data;
	if(slider->onValueChangedHandler)
		slider->onValueChangedHandler();
}

//Destructor
Slider::~Slider()
{
	MemFree(this->backend);
}

//Private methods
void Slider::System_CreateHandle()
{
	NOT_IMPLEMENTED_ERROR; //TODO: new implementation
	//this->backend = CreateWidgetPrivateData(gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 1), this);
	gtk_widget_show(THIS); //default to show

	g_signal_connect(THIS, "value-changed", G_CALLBACK(ValueChangedSlot), this);

	ADD_SELF_TO_PARENT(THIS);
}

//Public methods
uint32 Slider::GetPosition() const
{
	return (uint32) gtk_range_get_value(GTK_RANGE(THIS));
}

Size Slider::GetSizeHint() const
{
	return GetPreferedSizeGtk(THIS);
}

void Slider::SetMaximum(uint32 max)
{
	GtkAdjustment *adjustment = gtk_range_get_adjustment(GTK_RANGE(THIS));

	gtk_range_set_range(GTK_RANGE(THIS), gtk_adjustment_get_lower(adjustment), max);
}

void Slider::SetMinimum(uint32 min)
{
	GtkAdjustment *adjustment = gtk_range_get_adjustment(GTK_RANGE(THIS));

	gtk_range_set_range(GTK_RANGE(THIS), min, gtk_adjustment_get_upper(adjustment));
}

void Slider::SetPosition(uint32 pos)
{
	gtk_range_set_value(GTK_RANGE(THIS), pos);
}
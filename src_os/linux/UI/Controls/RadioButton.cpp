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
#include <ACStdLib/UI/Controls/RadioButton.hpp>
//Local
#include <ACStdLib/UI/WidgetContainer.hpp>
#include "../Gtk.h"
#include "../GtkEventQueue.hpp"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;
//Definitions
#define THIS (PRIVATE_DATA(this)->widget)

//Local functions
static GtkWidget *CreateRadioButtonInCorrectGroup(Widget *widget)
{
	for(const Widget *const& child : widget->GetParent()->GetChildren())
	{
		if(child == widget)
			break;
		if(IS_INSTANCE_OF(child, const RadioButton))
			return gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(PRIVATE_DATA(child)->widget));
	}
	return gtk_radio_button_new(nullptr);
}

//Destructor
RadioButton::~RadioButton()
{
	MemFree(this->systemHandle);
}

//Private methods
void RadioButton::System_CreateHandle()
{
	this->systemHandle = CreateWidgetPrivateData(CreateRadioButtonInCorrectGroup(this), this);
	gtk_widget_show(THIS); //default to show

	ADD_SELF_TO_PARENT(THIS);

	g_signal_connect(THIS, "clicked", G_CALLBACK(GtkEventQueue::ClickedSlot), this);
}

//Public methods
Size RadioButton::GetSizeHint() const
{
	return GetPreferedSizeGtk(THIS);
}

void RadioButton::SetText(const String &text)
{
	UTF8String textUTF8;

	textUTF8 = text.GetUTF16();
	gtk_button_set_label(GTK_BUTTON(THIS), (const gchar *)textUTF8.GetC_Str());
}
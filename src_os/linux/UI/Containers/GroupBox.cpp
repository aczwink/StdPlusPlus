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
#include <ACStdLib/UI/Containers/GroupBox.hpp>
//Local
#include <ACStdLib/UI/Layouts/GridLayout.hpp>
#include "../Gtk.h"
#include "../GtkEventQueue.hpp"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;
//Definitions
#define THIS PRIVATE_DATA(this)

//Destructor
GroupBox::~GroupBox()
{
	MemFree(this->systemHandle); //only free memory, don't destroy gtk objects because they get destroyed by the Window
}

//Private methods
void GroupBox::CreateOSHandle()
{
	this->systemHandle = CreateWidgetContainerPrivateData(gtk_frame_new(nullptr), this);

	gtk_widget_show(THIS->widget); //default to show

	ADD_SELF_TO_PARENT(THIS->widget);
}

//Public methods
Rect GroupBox::GetChildrenRect() const
{
	Rect rect;

	rect = this->GetBounds();

	//TODO: shit we dont know this correctly...
	rect.Enlarge(0, -15); //TODO: dx should be -10

	return rect;
}

void GroupBox::SetText(const String &text)
{
	UTF8String textUTF8;

	textUTF8 = text.GetUTF16();
	gtk_frame_set_label(GTK_FRAME(THIS->widget), (const gchar *)textUTF8.GetC_Str());
}
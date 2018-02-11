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
#include <ACStdLib/UI/Controls/Label.hpp>
//Local
#include "../Gtk.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;
//Definitions
#define THIS (PRIVATE_DATA(this)->widget)

//Destructor
Label::~Label()
{
	MemFree(this->backend);
}

//Private methods
void Label::System_CreateHandle()
{
	NOT_IMPLEMENTED_ERROR; //TODO: new implementation
	//this->backend = CreateWidgetPrivateData(gtk_label_new(nullptr), this);
	gtk_widget_show(THIS); //default to show

	//ADD_SELF_TO_PARENT(THIS);
}

//Public methods
Size Label::GetSizeHint() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: new implementation
	//return GetPreferedSizeGtk(THIS);
}

void Label::SetText(const OldString &text)
{
	UTF8String textUTF8 = text.GetUTF16();
	gtk_label_set_text(GTK_LABEL(THIS), (const gchar *) textUTF8.GetC_Str());
}
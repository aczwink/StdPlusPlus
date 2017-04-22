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
#include <ACStdLib/UI/Controls/PushButton.h>
//Local
#include "../Gtk.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;
//Definitions
#define THIS ((GtkWidget *)this->pOSHandle)

//Private methods
void PushButton::CreateOSHandle()
{
	this->pOSHandle = gtk_button_new();
	gtk_widget_show(THIS);

	ADD_SELF_TO_PARENT;
}

//Public methods
CSize PushButton::GetSizeHint() const
{
	NOT_IMPLEMENTED_ERROR;
	return CSize();
}

void PushButton::SetText(const CString &text)
{
	CUTF8String textUTF8;

	textUTF8 = text.GetUTF16();
	gtk_button_set_label(GTK_BUTTON(THIS), (const gchar *)textUTF8.GetC_Str());
}
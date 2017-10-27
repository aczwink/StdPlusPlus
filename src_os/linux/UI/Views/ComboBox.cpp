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
#include <ACStdLib/UI/Views/ComboBox.hpp>
//Local
#include <ACStdLib/UI/Controllers/Controller.hpp>
#include "../Gtk.h"
#include "../GtkEventQueue.hpp"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;
//Definitions
#define THIS (PRIVATE_DATA(this)->widget)

//Destructor
ComboBox::~ComboBox()
{
	MemFree(this->systemHandle);
}

//Public methods
Size ComboBox::GetSizeHint() const
{
	return GetPreferedSizeGtk(THIS);
}

//Private methods
void ComboBox::Backend_Create()
{
	this->systemHandle = CreateWidgetPrivateData(gtk_combo_box_new(), this);

	GtkCellRenderer *column = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(THIS), column, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(THIS), column, "text", 0, NULL);

	gtk_widget_show(THIS); //default to show

	ADD_SELF_TO_PARENT(THIS);
}

//Event handlers
void ComboBox::OnModelChanged()
{
	gtk_combo_box_set_model(GTK_COMBO_BOX(THIS), nullptr);

	if(this->controller)
	{
		GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);

		uint32 nEntries = this->controller->GetNumberOfChildren();
		UTF8String text;
		for(uint32 i = 0; i < nEntries; i++)
		{
			ControllerIndex childIndex = this->controller->GetChildIndex(i, 0, ControllerIndex());

			text = this->controller->GetText(childIndex).GetUTF16();
			GtkTreeIter iter;
			gtk_list_store_append(store, &iter);
			gtk_list_store_set(store, &iter, 0, text.GetC_Str(), -1);
		}

		gtk_combo_box_set_active(GTK_COMBO_BOX(THIS), 0);

		gtk_combo_box_set_model(GTK_COMBO_BOX(THIS), GTK_TREE_MODEL(store));
	}
}
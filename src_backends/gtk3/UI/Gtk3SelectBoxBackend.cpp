/*
 * Copyright (c) 2018-2021 Amir Czwink (amir130@hotmail.de)
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
#include "Gtk3SelectBoxBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
Gtk3SelectBoxBackend::Gtk3SelectBoxBackend(StdXX::UIBackend& uiBackend, SelectBox& selectBox)
    : ViewBackend(uiBackend), Gtk3WidgetBackend(uiBackend, gtk_combo_box_new()), WidgetBackend(uiBackend),
    selectBox(selectBox)
{
    GtkCellRenderer *column = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(this->GetGtkWidget()), column, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(this->GetGtkWidget()), column, u8"text", 0, NULL);
}

//Public methods
void Gtk3SelectBoxBackend::ControllerChanged()
{
	gtk_combo_box_set_model(GTK_COMBO_BOX(this->GetGtkWidget()), nullptr);

	const SharedPointer<TreeController>& controller = this->selectBox.GetController();

	if(!controller.IsNull())
	{
		GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);

		uint32 nEntries = controller->GetNumberOfChildren();

		for(uint32 i = 0; i < nEntries; i++)
		{
			ControllerIndex childIndex = controller->GetChildIndex(i, 0, ControllerIndex());

			GtkTreeIter iter;
			gtk_list_store_append(store, &iter);
			gtk_list_store_set(store, &iter, 0, controller->GetText(childIndex).ToUTF8().GetRawZeroTerminatedData(), -1);
		}

		gtk_combo_box_set_model(GTK_COMBO_BOX(this->GetGtkWidget()), GTK_TREE_MODEL(store));
	}
}

const Widget &Gtk3SelectBoxBackend::GetWidget() const
{
    return this->selectBox;
}

void Gtk3SelectBoxBackend::UpdateSelection() const
{
	const SelectionController& selectionController = this->selectBox.SelectionController();
	gtk_combo_box_set_active(GTK_COMBO_BOX(this->GetGtkWidget()), selectionController.GetSelectedIndexes()[0].GetRow());
}

//NOT IMPLEMENTED
void _stdxx_::Gtk3SelectBoxBackend::Repaint() {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
//END OF NOT IMPLEMENTED
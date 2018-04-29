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
#include <Std++/UI/Views/TableView.hpp>
//Global
#include <gtk/gtk.h>
#include <gobject/gvalue.h>
//Local
#include "../Gtk.h"
#include "../GtkEventSource.hpp"
#include <Std++/Containers/Array/FixedArray.hpp>
#include <Std++/UI/Controllers/TreeController.hpp>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;
//Definitions
#define THIS (PRIVATE_DATA(this)->widget)

//Destructor
TableView::~TableView()
{
	MemFree(this->backend);
}

//Private methods
void TableView::Backend_Create()
{
	NOT_IMPLEMENTED_ERROR; //TODO: new implementation
	//this->backend = CreateWidgetPrivateData(gtk_tree_view_new(), this);

	gtk_widget_set_hexpand(THIS, TRUE);
	gtk_widget_set_vexpand(THIS, TRUE);
	gtk_widget_show_all(THIS); //default is show

	//ADD_SELF_TO_PARENT(THIS);

	//signals
	g_signal_connect(gtk_tree_view_get_selection(GTK_TREE_VIEW(THIS)), "changed", G_CALLBACK(GtkEventSource::ChangedSlot), this);
}

//Event handlers
void TableView::OnModelChanged()
{
	gtk_tree_view_set_model(GTK_TREE_VIEW(THIS), nullptr);

	//clear all columns
	while(true)
	{
		GtkTreeViewColumn *column = gtk_tree_view_get_column(GTK_TREE_VIEW(THIS), 0);
		if(!column)
			break;

		gtk_tree_view_remove_column(GTK_TREE_VIEW(THIS), column);
	}

	if(this->controller)
	{
		//add columns
		uint32 nCols = this->controller->GetNumberOfColumns();
		ASSERT(nCols, "A table must have at least one column.");
		GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
		for (uint32 i = 0; i < nCols; i++)
		{
			GtkTreeViewColumn *column = column = gtk_tree_view_column_new_with_attributes((gchar *)this->controller->GetColumnText(i).ToUTF8().GetRawZeroTerminatedData(), renderer, "text", i, nullptr);

			gtk_tree_view_append_column(GTK_TREE_VIEW(THIS), column);
			//gtk_tree_view_column_pack_start(column, renderer, TRUE);
		}

		//fill model
		FixedArray<GType> types(nCols);
		for(uint32 i = 0; i < nCols; i++)
			types[i] = G_TYPE_STRING;
		GtkListStore *store = gtk_list_store_newv(nCols, &types[0]);

		uint32 nRows = this->controller->GetNumberOfChildren();
		for(uint32 i = 0; i < nRows; i++)
		{
			GtkTreeIter iter;
			gtk_list_store_append(store, &iter);
			for(uint32 j = 0; j < nCols; j++)
			{
				ControllerIndex cellIndex = this->controller->GetChildIndex(i, j);

				gtk_list_store_set(store, &iter, j, this->controller->GetText(cellIndex).ToUTF8().GetRawZeroTerminatedData(), -1);
			}
		}

		gtk_tree_view_set_model(GTK_TREE_VIEW(THIS), GTK_TREE_MODEL(store));
	}
}
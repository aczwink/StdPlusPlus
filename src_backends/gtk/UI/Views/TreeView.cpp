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
#include <ACStdLib/UI/Views/TreeView.hpp>
//Global
#include <gtk/gtk.h>
//Local
#include "../Gtk.h"
#include "../GtkEventQueue.hpp"
#include <ACStdLib/UI/Controllers/TreeController.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;
//Definitions
#define THIS (PRIVATE_DATA(this)->widget)

//Local functions
static void AddNodes(GtkTreeStore *store, GtkTreeIter *nodeIter, const ControllerIndex &parent, TreeController &controller)
{
    uint32 nChildren = controller.GetNumberOfChildren(parent);
    for(uint32 i = 0; i < nChildren; i++)
    {
        ControllerIndex childIndex = controller.GetChildIndex(i, 0, parent);

		GtkTreeIter childIter;
        gtk_tree_store_append(store, &childIter, nodeIter);
        gtk_tree_store_set(store, &childIter, 0, controller.GetText(childIndex).ToUTF8().GetRawZeroTerminatedData(), -1);

		AddNodes(store, &childIter, childIndex, controller);
    }
}

//Destructor
TreeView::~TreeView()
{
	MemFree(this->backend);
}

//Eventhandlers
void TreeView::OnModelChanged()
{
	gtk_tree_view_set_model(GTK_TREE_VIEW(THIS), nullptr);

	if(this->controller)
	{
		GtkTreeStore *store;

		store = gtk_tree_store_new(1, G_TYPE_STRING);

		AddNodes(store, nullptr, ControllerIndex(), *this->controller);

		gtk_tree_view_set_model(GTK_TREE_VIEW(THIS), GTK_TREE_MODEL(store));
	}
}

void TreeView::OnSelectionChanged()
{
	this->selectionController.ClearSelection();

	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(THIS));
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(THIS));
	GList *selectedRows = gtk_tree_selection_get_selected_rows(selection, &model);
	while(selectedRows)
	{
		GtkTreePath *path = static_cast<GtkTreePath *>(selectedRows->data);
		gint depth = gtk_tree_path_get_depth(path);
		gint *indices = gtk_tree_path_get_indices(path);

		ControllerIndex index;
		for(gint i = 0; i < depth; i++)
			index = this->controller->GetChildIndex(indices[i], 0, index);
		this->selectionController.Select(index);

		selectedRows = selectedRows->next;
	}
	g_list_free_full (selectedRows, (GDestroyNotify) gtk_tree_path_free);

	View::OnSelectionChanged();
}

//Private methods
void TreeView::CreateOSWindow()
{
	NOT_IMPLEMENTED_ERROR; //TODO: new implementation
    //this->backend = CreateWidgetPrivateData(gtk_tree_view_new(), this);
    gtk_widget_set_vexpand(THIS, TRUE);

	ADD_SELF_TO_PARENT(THIS);

    //append the standard column
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    renderer = gtk_cell_renderer_text_new();

    column = gtk_tree_view_column_new_with_attributes("", renderer, "text", 0, nullptr);
    gtk_tree_view_column_set_sort_column_id(column, 0);
    gtk_tree_view_append_column(GTK_TREE_VIEW(THIS), column);
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(THIS), false); //make it invisible


    gtk_widget_show_all(THIS); //default is show

	//signals
	g_signal_connect(gtk_tree_view_get_selection(GTK_TREE_VIEW(THIS)), "changed", G_CALLBACK(GtkEventQueue::ChangedSlot), this);
}
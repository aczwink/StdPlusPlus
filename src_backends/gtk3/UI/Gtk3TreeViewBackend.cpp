/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include "Gtk3TreeViewBackend.hpp"
//Local
#include <Std++/Containers/Array/FixedArray.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
Gtk3TreeViewBackend::Gtk3TreeViewBackend(UIBackend& uiBackend, TreeView& treeView) : ViewBackend(uiBackend),
	Gtk3WidgetBackend(uiBackend, gtk_tree_view_new()), WidgetBackend(uiBackend), treeView(treeView)
{
	GtkWidget* gtkWidget = this->GetGtkWidget();

	gtk_widget_set_hexpand(gtkWidget, TRUE);
	gtk_widget_set_vexpand(gtkWidget, TRUE);

	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(gtkWidget), true);

	gtk_widget_show_all(gtkWidget); //default is show

	g_object_set_data(G_OBJECT(gtkWidget), u8"Std++", &treeView);
}

//Public methods
void Gtk3TreeViewBackend::ControllerChanged()
{
	//reset view
	gtk_tree_view_set_model(GTK_TREE_VIEW(this->GetGtkWidget()), nullptr);

	//clear all columns
	while(true)
	{
		GtkTreeViewColumn *column = gtk_tree_view_get_column(GTK_TREE_VIEW(this->GetGtkWidget()), 0);
		if(!column)
			break;

		gtk_tree_view_remove_column(GTK_TREE_VIEW(this->GetGtkWidget()), column);
	}

	if(this->treeView.HasController())
	{
		const SharedPointer<TreeController>& controller = this->treeView.GetController();

		//add columns
		uint32 nCols = controller->GetNumberOfColumns();
		ASSERT(nCols, u8"A TreeView must have at least one column.");
		GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
		for (uint32 i = 0; i < nCols; i++)
		{
			GtkTreeViewColumn *column = gtk_tree_view_column_new();
			gtk_tree_view_column_set_title(column, (gchar *)controller->GetColumnText(i).ToUTF8().GetRawZeroTerminatedData());
			gtk_tree_view_column_pack_start(column, renderer, TRUE);
			gtk_tree_view_column_add_attribute(column, renderer, u8"text", i);

			gtk_tree_view_append_column(GTK_TREE_VIEW(this->GetGtkWidget()), column);
		}

		//fill model
		FixedArray<GType> types(nCols);
		for(uint32 i = 0; i < nCols; i++)
			types[i] = G_TYPE_STRING;

		GtkTreeStore *store = gtk_tree_store_newv(nCols, &types[0]);

		this->AddNodes(store, nullptr, ControllerIndex(), *controller);

		gtk_tree_view_set_model(GTK_TREE_VIEW(this->GetGtkWidget()), GTK_TREE_MODEL(store));
	}
}

Widget &Gtk3TreeViewBackend::GetWidget()
{
	return this->treeView;
}

const Widget &Gtk3TreeViewBackend::GetWidget() const
{
	return this->treeView;
}

//Private methods
void Gtk3TreeViewBackend::AddNodes(GtkTreeStore *store, GtkTreeIter *nodeIter, const ControllerIndex &parent, const TreeController &controller) const
{
	uint32 nCols = controller.GetNumberOfColumns();
	uint32 nChildren = controller.GetNumberOfChildren(parent);
	for(uint32 i = 0; i < nChildren; i++)
	{
		GtkTreeIter childIter;
		gtk_tree_store_append(store, &childIter, nodeIter);
		for (uint32 j = 0; j < nCols; j++)
		{
			ControllerIndex childIndex = controller.GetChildIndex(i, j, parent);
			gtk_tree_store_set(store, &childIter, j, controller.GetText(childIndex).ToUTF8().GetRawZeroTerminatedData(), -1);
		}

		ControllerIndex childIndex = controller.GetChildIndex(i, Unsigned<uint32>::Max(), parent);
		this->AddNodes(store, &childIter, childIndex, controller);
	}
}







//NOT IMPLEMENTED
void _stdxx_::Gtk3TreeViewBackend::UpdateSelection() const {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::Gtk3TreeViewBackend::Repaint() {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::Gtk3TreeViewBackend::SetEditable(bool enable) const {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
//NOT IMPLEMENTED
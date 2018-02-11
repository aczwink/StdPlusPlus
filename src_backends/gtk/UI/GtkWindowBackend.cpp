/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include "GtkWindowBackend.hpp"
//Local
#include <ACStdLib/UI/Controllers/TreeController.hpp>
#include <ACStdLib/Containers/Array/FixedArray.hpp>
#include "_RedirectGtkContainer.h"
#include "GtkEventQueue.hpp"
//Namespaces
using namespace _ACStdLib_internal;

struct FileChooserData
{
	const Function<bool(Path &)> *callback;
	GtkWidget *acceptButton;
};

static void SelectionChanged(GtkFileChooser *fileChooser, gpointer user_data)
{
	bool accept = false;
	FileChooserData *fcd = static_cast<FileChooserData *>(user_data);
	char *fileName = gtk_file_chooser_get_filename(fileChooser);
	if(fileName)
	{
		Path path = String(fileName);
		accept = (*fcd->callback)(path);
		g_free(fileName);
	}

	gtk_widget_set_sensitive(fcd->acceptButton, accept);
};

//Constructor
GtkWindowBackend::GtkWindowBackend(_ACStdLib_internal::WindowBackendType type, Widget *widget) : type(type), widget(widget)
{
	bool isContainer = false;
	switch(type)
	{
		case WindowBackendType::ComboBox:
		{
			this->gtkWidget = gtk_combo_box_new();

			GtkCellRenderer *column = gtk_cell_renderer_text_new();
			gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(this->gtkWidget), column, TRUE);
			gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(this->gtkWidget), column, u8"text", 0, NULL);

			g_signal_connect(this->gtkWidget, u8"changed", G_CALLBACK(GtkEventQueue::ChangedSlot), widget);
		}
		break;
		case WindowBackendType::GroupBox:
		{
			isContainer = true;
			this->gtkWidget = gtk_frame_new(nullptr);
		}
		break;
		case WindowBackendType::PushButton:
		{
			this->gtkWidget = gtk_button_new();

			g_signal_connect(this->gtkWidget, u8"clicked", G_CALLBACK(GtkEventQueue::ClickedSlot), widget);
		}
		break;
		case WindowBackendType::SearchBox:
		{
			this->gtkWidget = gtk_search_entry_new();
		}
		break;
		case WindowBackendType::TreeView:
		{
			this->gtkWidget = gtk_tree_view_new();

			gtk_widget_set_hexpand(this->gtkWidget, TRUE);
			gtk_widget_set_vexpand(this->gtkWidget, TRUE);

			gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(this->gtkWidget), true);

			gtk_widget_show_all(this->gtkWidget); //default is show

			//signals
			g_signal_connect(gtk_tree_view_get_selection(GTK_TREE_VIEW(this->gtkWidget)), u8"changed", G_CALLBACK(GtkEventQueue::TreeSelectionSlot), widget);
		}
		break;
		case WindowBackendType::Window:
		{
			isContainer = true;
			this->gtkWidget = gtk_window_new(GTK_WINDOW_TOPLEVEL);

			this->headerBar = gtk_header_bar_new();
			gtk_header_bar_set_has_subtitle(GTK_HEADER_BAR(this->headerBar), false);
			gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(this->headerBar), true);
			gtk_widget_show(this->headerBar); //default to show

			gtk_window_set_titlebar(GTK_WINDOW(this->gtkWidget), this->headerBar);

			//g_signal_connect(this->gtkWidget, u8"check-resize", G_CALLBACK(GtkEventQueue::CheckResizeSlot), widget);
			g_signal_connect(this->gtkWidget, u8"delete-event", G_CALLBACK(GtkEventQueue::CloseSlot), nullptr);
			g_signal_connect(this->gtkWidget, u8"destroy", G_CALLBACK(GtkEventQueue::DestroySlot), widget);

			gtk_window_set_position(GTK_WINDOW(this->gtkWidget), GTK_WIN_POS_CENTER);
		}
		break;
	}

	g_signal_connect(this->gtkWidget, u8"size-allocate", G_CALLBACK(GtkEventQueue::SizeAllocateSlot), widget);

	if(!GTK_IS_WINDOW(this->gtkWidget))
		gtk_widget_show(this->gtkWidget); //default to show

	g_object_set_data(G_OBJECT(this->gtkWidget), u8"ACStdLib", widget);

	this->childAreaWidget = nullptr;
	if(isContainer)
	{
		this->childAreaWidget = redirect_container_new();
		g_object_set_data(G_OBJECT(this->childAreaWidget), u8"ACStdLib", widget);
		gtk_container_add(GTK_CONTAINER(this->gtkWidget), this->childAreaWidget);
		gtk_widget_show(this->childAreaWidget); //default to show
	}
}

//Destructor
GtkWindowBackend::~GtkWindowBackend()
{
	if(this->childAreaWidget)
		gtk_widget_destroy(GTK_WIDGET(this->childAreaWidget));

	gtk_widget_destroy(GTK_WIDGET(this->gtkWidget));
}

//Public methods
void GtkWindowBackend::ClearView() const
{
	View *view = (View *)this->widget;
	const TreeController *controller = view->GetController();

	switch(this->type)
	{
		case WindowBackendType::ComboBox:
		{
			gtk_combo_box_set_model(GTK_COMBO_BOX(this->gtkWidget), nullptr);

			if(controller)
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

				gtk_combo_box_set_model(GTK_COMBO_BOX(this->gtkWidget), GTK_TREE_MODEL(store));
			}
		}
		break;
		case WindowBackendType::TreeView:
		{
			gtk_tree_view_set_model(GTK_TREE_VIEW(this->gtkWidget), nullptr);

			//clear all columns
			while(true)
			{
				GtkTreeViewColumn *column = gtk_tree_view_get_column(GTK_TREE_VIEW(this->gtkWidget), 0);
				if(!column)
					break;

				gtk_tree_view_remove_column(GTK_TREE_VIEW(this->gtkWidget), column);
			}

			if(controller)
			{
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

					gtk_tree_view_append_column(GTK_TREE_VIEW(this->gtkWidget), column);
				}

				//fill model
				FixedArray<GType> types(nCols);
				for(uint32 i = 0; i < nCols; i++)
					types[i] = G_TYPE_STRING;

				GtkTreeStore *store = gtk_tree_store_newv(nCols, &types[0]);

				this->AddNodes(store, nullptr, ControllerIndex(), *controller);

				gtk_tree_view_set_model(GTK_TREE_VIEW(this->gtkWidget), GTK_TREE_MODEL(store));
			}
		}
		break;
	}
}

WindowBackend *GtkWindowBackend::CreateChildBackend(_ACStdLib_internal::WindowBackendType type, Widget *widget) const
{
	GtkWindowBackend *child = new GtkWindowBackend(type, widget);

	gtk_container_add(GTK_CONTAINER(this->childAreaWidget), child->gtkWidget);

	return child;
}

Size GtkWindowBackend::GetSize() const
{
	GtkAllocation alloc;

	gtk_widget_get_allocation(this->gtkWidget, &alloc);

	return Size((uint16)alloc.width, (uint16)alloc.height);
}

Size GtkWindowBackend::GetSizeHint() const
{
	int min1, nat1, min2, nat2;

	gtk_widget_get_preferred_width(this->gtkWidget, &min1, &nat1);
	gtk_widget_get_preferred_height(this->gtkWidget, &min2, &nat2);

	return Size(nat1, nat2);
}

void GtkWindowBackend::Select(ControllerIndex &controllerIndex) const
{
	gtk_combo_box_set_active(GTK_COMBO_BOX(this->gtkWidget), controllerIndex.GetRow());
}

Path GtkWindowBackend::SelectExistingDirectory(const String &title, const Function<bool(Path &)> callback) const
{
	GtkWidget *fileChooserDialog = gtk_file_chooser_dialog_new((gchar *)title.ToUTF8().GetRawZeroTerminatedData(), GTK_WINDOW(this->gtkWidget), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, "Cancel", GTK_RESPONSE_CANCEL, "Select", GTK_RESPONSE_ACCEPT, nullptr);

	FileChooserData fcd;
	fcd.callback = &callback;
	fcd.acceptButton = gtk_dialog_get_widget_for_response(GTK_DIALOG(fileChooserDialog), GTK_RESPONSE_ACCEPT);
	g_signal_connect(fileChooserDialog, "selection-changed", G_CALLBACK(SelectionChanged), (gpointer)&fcd);

	gint result = gtk_dialog_run(GTK_DIALOG(fileChooserDialog));
	String fileName;
	if(result == GTK_RESPONSE_ACCEPT)
	{
		char *pFileName;

		pFileName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileChooserDialog));
		fileName = String::CopyRawString(pFileName);

		g_free(pFileName);
	}

	gtk_widget_destroy(fileChooserDialog);

	return fileName;
}

void GtkWindowBackend::SetBounds(const ACStdLib::Rect &area)
{
	if(GTK_IS_WINDOW(this->gtkWidget))
		gtk_window_set_default_size(GTK_WINDOW(this->gtkWidget), area.width(), area.height());
	else
		gtk_widget_queue_resize(this->gtkWidget);
}

void GtkWindowBackend::SetEnabled(bool enable) const
{
	gtk_widget_set_sensitive(this->gtkWidget, enable);
}

void GtkWindowBackend::SetHint(const String &text) const
{
	gtk_widget_set_tooltip_text(this->gtkWidget, reinterpret_cast<const gchar *>(text.ToUTF8().GetRawZeroTerminatedData()));
}

void GtkWindowBackend::SetText(const String &text)
{
	switch(this->type)
	{
		case WindowBackendType::PushButton:
			gtk_button_set_label(GTK_BUTTON(this->gtkWidget), reinterpret_cast<const gchar *>(text.ToUTF8().GetRawZeroTerminatedData()));
			break;
		case WindowBackendType::Window:
			gtk_window_set_title(GTK_WINDOW(this->gtkWidget), reinterpret_cast<const gchar *>(text.ToUTF8().GetRawZeroTerminatedData()));
			gtk_header_bar_set_title(GTK_HEADER_BAR(this->headerBar), reinterpret_cast<const gchar *>(text.ToUTF8().GetRawZeroTerminatedData()));
			break;
	}
}

void GtkWindowBackend::Show(bool visible)
{
	if(visible)
		gtk_widget_show(this->gtkWidget);
	else
		gtk_widget_hide(this->gtkWidget);
}

void GtkWindowBackend::ShowInformationBox(const String &title, const String &message) const
{
	GtkWidget *widget = gtk_message_dialog_new(GTK_WINDOW(this->gtkWidget), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, (const gchar *) message.ToUTF8().GetRawZeroTerminatedData());
	gtk_window_set_title(GTK_WINDOW(widget), (const gchar *) title.ToUTF8().GetRawZeroTerminatedData());
	gtk_dialog_run(GTK_DIALOG(widget));
	gtk_widget_destroy(widget);
}

void GtkWindowBackend::UpdateSelection(SelectionController &selectionController) const
{
	View *view = (View *)this->widget;
	const TreeController *controller = view->GetController();

	switch(this->type)
	{
		case WindowBackendType::ComboBox:
		{
			selectionController.ClearSelection();
			gint index = gtk_combo_box_get_active(GTK_COMBO_BOX(this->gtkWidget));
			ControllerIndex childIndex = controller->GetChildIndex(index, 0, ControllerIndex());
			selectionController.Select(childIndex);
		}
		break;
		case WindowBackendType::TreeView:
		{
			selectionController.ClearSelection();

			GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(this->gtkWidget));
			GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(this->gtkWidget));
			GList *selectedRows = gtk_tree_selection_get_selected_rows(selection, &model);
			while(selectedRows)
			{
				GtkTreePath *path = static_cast<GtkTreePath *>(selectedRows->data);
				gint depth = gtk_tree_path_get_depth(path);
				gint *indices = gtk_tree_path_get_indices(path);

				ControllerIndex index;
				for(gint i = 0; i < depth; i++)
					index = controller->GetChildIndex(indices[i], 0, index);
				selectionController.Select(index);

				selectedRows = selectedRows->next;
			}
			g_list_free_full (selectedRows, (GDestroyNotify) gtk_tree_path_free);
		}
		break;
	}
}

//Private methods
void GtkWindowBackend::AddNodes(GtkTreeStore *store, GtkTreeIter *nodeIter, const ControllerIndex &parent, const TreeController &controller) const
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

		ControllerIndex childIndex = controller.GetChildIndex(i, Natural<uint32>::Max(), parent);
		this->AddNodes(store, &childIter, childIndex, controller);
	}
}
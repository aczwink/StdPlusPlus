/*
 * Copyright (c) 2018-2019,2021 Amir Czwink (amir130@hotmail.de)
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
#include "oldGtkWindowBackend.hpp"
//Local
#include <Std++/UI/Controllers/TreeController.hpp>
#include <Std++/Containers/Array/FixedArray.hpp>
#include <Std++/Signed.hpp>
#include "_RedirectGtkContainer.h"
#include "GtkEventSource.hpp"
//Namespaces
using namespace _stdpp;

//Constructor
GtkWindowBackend::GtkWindowBackend(UIBackend *uiBackend, _stdpp::WindowBackendType type, Widget *widget, const GtkWindowBackend *parentBackend) : WindowBackend(uiBackend, type, widget)
{
	bool isContainer = false;
	switch(type)
	{
		case WindowBackendType::ComboBox:
		{
			g_signal_connect(this->gtkWidget, u8"changed", G_CALLBACK(GtkEventSource::ChangedSlot), this->widget);
		}
		break;
		case WindowBackendType::RadioButton:
		{
			GtkWidget *buttonGroupWidget = nullptr;
			GList *child = gtk_container_get_children(GTK_CONTAINER(parentBackend->childAreaWidget));
			while((child = g_list_next(child)) != nullptr)
			{
				GtkWidget *childWidget = static_cast<GtkWidget *>(child->data);
				if(GTK_IS_RADIO_BUTTON(childWidget))
				{
					buttonGroupWidget = childWidget;
					break;
				}
			}

			if(buttonGroupWidget)
				this->gtkWidget = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(buttonGroupWidget));
			else
				this->gtkWidget = gtk_radio_button_new(nullptr);

			g_signal_connect(this->gtkWidget, u8"clicked", G_CALLBACK(GtkEventSource::ClickedSlot), this->widget);
		}
		break;
		case WindowBackendType::RenderTarget:
		{
			gtk_widget_add_events(this->gtkWidget, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK | GDK_SCROLL_MASK);
			g_signal_connect(this->gtkWidget, u8"button-press-event", G_CALLBACK(GtkEventSource::ButtonSlot), this);
			g_signal_connect(this->gtkWidget, u8"button-release-event", G_CALLBACK(GtkEventSource::ButtonSlot), this);
			g_signal_connect(this->gtkWidget, u8"motion-notify-event", G_CALLBACK(GtkEventSource::MouseMotionSlot), this);
			g_signal_connect(this->gtkWidget, u8"scroll-event", G_CALLBACK(GtkEventSource::ScrollSlot), this->widget);
		}
		break;
		case WindowBackendType::SearchBox:
		{
			this->gtkWidget = gtk_search_entry_new();
		}
		break;
		case WindowBackendType::SpinBox:
		{
			//https://developer.gnome.org/gtk3/stable/GtkSpinButton.html
			this->gtkWidget = gtk_spin_button_new(gtk_adjustment_new(0, Integer<int32>::Min(), Integer<int32>::Max(), 1, 5, 5), 1, 0);
		}
		break;
		case WindowBackendType::TreeView:
		{
			//signals
			g_signal_connect(gtk_tree_view_get_selection(GTK_TREE_VIEW(this->gtkWidget)), u8"changed", G_CALLBACK(GtkEventSource::TreeSelectionSlot), widget);
		}
		break;
		case WindowBackendType::Window:
		{
			isContainer = true;
			this->headerBar = gtk_header_bar_new();
			gtk_header_bar_set_has_subtitle(GTK_HEADER_BAR(this->headerBar), false);
			gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(this->headerBar), true);
			gtk_widget_show(this->headerBar); //default to show

			gtk_window_set_titlebar(GTK_WINDOW(this->gtkWidget), this->headerBar);

			//g_signal_connect(this->gtkWidget, u8"check-resize", G_CALLBACK(GtkEventSource::CheckResizeSlot), widget);

			gtk_window_set_position(GTK_WINDOW(this->gtkWidget), GTK_WIN_POS_CENTER);
		}
		break;
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
uint32 GtkWindowBackend::GetPosition() const
{
	return (uint32) gtk_range_get_value(GTK_RANGE(this->gtkWidget));
}

void GtkWindowBackend::GetRange(int32 &min, int32 &max)
{
	gdouble gmin, gmax;
	gtk_spin_button_get_range(GTK_SPIN_BUTTON(this->gtkWidget), &gmin, &gmax);
	min = static_cast<int32>(gmin);
	max = static_cast<int32>(gmax);
}

Size GtkWindowBackend::GetSize() const
{
	GtkAllocation alloc;

	gtk_widget_get_allocation(this->gtkWidget, &alloc);

	return Size((uint16)alloc.width, (uint16)alloc.height);
}

int32 GtkWindowBackend::GetValue() const
{
	return gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->gtkWidget));
}

bool GtkWindowBackend::IsChecked() const
{
	return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(this->gtkWidget)) != 0;
}

void GtkWindowBackend::Paint()
{
}

void GtkWindowBackend::SetBounds(const Rect &area)
{
	GtkEventSource::EmitResizingEvent(*this->widget, area);
	if(this->childAreaWidget)
		gtk_container_check_resize(GTK_CONTAINER(this->childAreaWidget));

	//printf("SetBounds: %p -> %d, %d, %d, %d\n", this, area.x(), area.y(), area.width(), area.height());
}

void GtkWindowBackend::SetRange(int32 min, int32 max)
{
	gtk_spin_button_set_range(GTK_SPIN_BUTTON(this->gtkWidget), min, max);
}

void GtkWindowBackend::SetText(const String &text)
{
	const gchar *gtkText = reinterpret_cast<const gchar *>(text.ToUTF8().GetRawZeroTerminatedData());
	switch(this->type)
	{
		case WindowBackendType::RadioButton:
			gtk_button_set_label(GTK_BUTTON(this->gtkWidget), gtkText);
			break;
		case WindowBackendType::Window:
			gtk_header_bar_set_title(GTK_HEADER_BAR(this->headerBar), gtkText);
			break;
	}
}

void GtkWindowBackend::SetValue(int32 value)
{
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(this->gtkWidget), value);
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
void GtkWindowBackend::SetMenuBar(StdPlusPlus::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend)
{
}

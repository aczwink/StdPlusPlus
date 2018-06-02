/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include "GtkWindowBackend.hpp"
//Local
#include <Std++/UI/Controllers/TreeController.hpp>
#include <Std++/Containers/Array/FixedArray.hpp>
#include <Std++/Integer.hpp>
#include "_RedirectGtkContainer.h"
#include "GtkEventSource.hpp"
//Namespaces
using namespace _stdpp;

struct FileChooserData
{
	const Function<bool(Path &)> *callback;
	GtkWidget *acceptButton;
};

//Local functions
static void OnRealize(GtkWidget *gtkWidget, gpointer user_data)
{
	GtkGLArea *glArea = GTK_GL_AREA(gtkWidget);

	gtk_gl_area_make_current(glArea);

	//enable depth buffer
	gtk_gl_area_set_has_depth_buffer(glArea, true);
	//glDisable(GL_DEPTH_TEST); //stupid gtk3!!! GL_DEPTH_TEST is disabled by default even if the def framebuffer has a depth buffer
}

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
GtkWindowBackend::GtkWindowBackend(UIBackend *uiBackend, _stdpp::WindowBackendType type, Widget *widget, const GtkWindowBackend *parentBackend) : WindowBackend(uiBackend, type, widget)
{
	bool isContainer = false;
	switch(type)
	{
		case WindowBackendType::CheckBox:
		{
			this->gtkWidget = gtk_check_button_new();

			g_signal_connect(this->gtkWidget, u8"toggled", G_CALLBACK(GtkEventSource::ToggledSlot), this->widget);
		}
		break;
		case WindowBackendType::ComboBox:
		{
			this->gtkWidget = gtk_combo_box_new();

			GtkCellRenderer *column = gtk_cell_renderer_text_new();
			gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(this->gtkWidget), column, TRUE);
			gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(this->gtkWidget), column, u8"text", 0, NULL);

			g_signal_connect(this->gtkWidget, u8"changed", G_CALLBACK(GtkEventSource::ChangedSlot), this->widget);
		}
		break;
		case WindowBackendType::GroupBox:
		{
			isContainer = true;
			this->gtkWidget = gtk_frame_new(nullptr);
		}
		break;
		case WindowBackendType::Label:
		{
			this->gtkWidget = gtk_label_new(nullptr);
		}
		break;
		case WindowBackendType::ListView:
		{
			this->gtkWidget = gtk_list_box_new();
		}
		break;
		case WindowBackendType::PushButton:
		{
			this->gtkWidget = gtk_button_new();

			g_signal_connect(this->gtkWidget, u8"clicked", G_CALLBACK(GtkEventSource::ClickedSlot), widget);
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
			//TODO: own gtk3 gl area because: GL_DEPTH_TEST is enabled by gtk3, when a frame buffer has a depth buffer-.-
			//this->backend = CreateWidgetPrivateData(ac_gtk_opengl_widget_new(), this);
			//ac_gtk_opengl_widget_setwidget(AC_GTK_OPENGL_WIDGET(THIS), this);

			this->gtkWidget = gtk_gl_area_new();

			g_signal_connect(this->gtkWidget, u8"realize", G_CALLBACK(OnRealize), this);
			g_signal_connect(this->gtkWidget, u8"render", G_CALLBACK(GtkEventSource::PaintSlot), this->widget);

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
		case WindowBackendType::Slider:
		{
			this->gtkWidget = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);

			g_signal_connect(this->gtkWidget, u8"value-changed", G_CALLBACK(GtkEventSource::ValueChangedSlot), this->widget);
		}
		break;
		case WindowBackendType::SpinBox:
		{
			//https://developer.gnome.org/gtk3/stable/GtkSpinButton.html
			this->gtkWidget = gtk_spin_button_new(gtk_adjustment_new(0, Integer<int32>::Min(), Integer<int32>::Max(), 1, 5, 5), 1, 0);
		}
		break;
		case WindowBackendType::TextEdit:
		{
			this->gtkWidget = gtk_text_view_new();
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
			g_signal_connect(gtk_tree_view_get_selection(GTK_TREE_VIEW(this->gtkWidget)), u8"changed", G_CALLBACK(GtkEventSource::TreeSelectionSlot), widget);
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

			//g_signal_connect(this->gtkWidget, u8"check-resize", G_CALLBACK(GtkEventSource::CheckResizeSlot), widget);
			g_signal_connect(this->gtkWidget, u8"delete-event", G_CALLBACK(GtkEventSource::CloseSlot), nullptr);

			gtk_window_set_position(GTK_WINDOW(this->gtkWidget), GTK_WIN_POS_CENTER);
		}
		break;
	}

	g_signal_connect(this->gtkWidget, u8"size-allocate", G_CALLBACK(GtkEventSource::SizeAllocateSlot), widget);

	if(!GTK_IS_WINDOW(this->gtkWidget))
		gtk_widget_show(this->gtkWidget); //default to show

	g_object_set_data(G_OBJECT(this->gtkWidget), u8"Std++", widget);

	this->childAreaWidget = nullptr;
	if(isContainer)
	{
		this->childAreaWidget = redirect_container_new();
		g_object_set_data(G_OBJECT(this->childAreaWidget), u8"Std++", widget);
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

WindowBackend *GtkWindowBackend::CreateChildBackend(_stdpp::WindowBackendType type, Widget *widget) const
{
	GtkWindowBackend *child = new GtkWindowBackend(this->GetUIBackend(), type, widget, this);

	gtk_container_add(GTK_CONTAINER(this->childAreaWidget), child->gtkWidget);

	return child;
}

Rect GtkWindowBackend::GetChildrenRect() const
{
	GtkAllocation alloc;

	gtk_widget_get_allocation(this->childAreaWidget, &alloc);

	return Rect(alloc.x, alloc.y, alloc.width, alloc.height);
	/*
	 * Rect rect;

	rect = this->GetBounds();

	//TODO: shit we dont know this correctly...
	rect.y() += 10;
	rect.height() -= 10;

	return rect;
	 */
}

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

Size GtkWindowBackend::GetSizeHint() const
{
	int min1, nat1, min2, nat2;

	if(IS_REDIRECT_CONTAINER(this->childAreaWidget))
		return Size();

	gtk_widget_get_preferred_width(this->gtkWidget, &min1, &nat1);
	gtk_widget_get_preferred_height(this->gtkWidget, &min2, &nat2);

	return Size(nat1, nat2);
}

int32 GtkWindowBackend::GetValue() const
{
	return gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(this->gtkWidget));
}

bool GtkWindowBackend::IsChecked() const
{
	return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(this->gtkWidget)) != 0;
}

void GtkWindowBackend::Maximize()
{
	gtk_window_maximize(GTK_WINDOW(this->gtkWidget));
	this->Show(true);
}

void GtkWindowBackend::Paint()
{
}

void GtkWindowBackend::Repaint()
{
	gtk_widget_queue_draw(this->gtkWidget);
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

void GtkWindowBackend::SetBounds(const Rect &area)
{
	gtk_widget_queue_resize(this->gtkWidget);
	if(this->childAreaWidget)
		gtk_container_check_resize(GTK_CONTAINER(this->childAreaWidget));

	//printf("SetBounds: %p -> %d, %d, %d, %d\n", this, area.x(), area.y(), area.width(), area.height());
}

void GtkWindowBackend::SetEditable(bool enable) const
{
	gtk_text_view_set_editable(GTK_TEXT_VIEW(this->gtkWidget), enable);
}

void GtkWindowBackend::SetEnabled(bool enable) const
{
	gtk_widget_set_sensitive(this->gtkWidget, enable);
}

void GtkWindowBackend::SetHint(const String &text) const
{
	gtk_widget_set_tooltip_text(this->gtkWidget, reinterpret_cast<const gchar *>(text.ToUTF8().GetRawZeroTerminatedData()));
}

void GtkWindowBackend::SetMaximum(uint32 max)
{
	GtkAdjustment *adjustment = gtk_range_get_adjustment(GTK_RANGE(this->gtkWidget));

	gtk_range_set_range(GTK_RANGE(this->gtkWidget), gtk_adjustment_get_lower(adjustment), max);
}

void GtkWindowBackend::SetMinimum(uint32 min)
{
	GtkAdjustment *adjustment = gtk_range_get_adjustment(GTK_RANGE(this->gtkWidget));

	gtk_range_set_range(GTK_RANGE(this->gtkWidget), min, gtk_adjustment_get_upper(adjustment));
}

void GtkWindowBackend::SetPosition(uint32 pos) const
{
	gtk_range_set_value(GTK_RANGE(this->gtkWidget), pos);
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
		case WindowBackendType::CheckBox:
			gtk_button_set_label(GTK_BUTTON(this->gtkWidget), gtkText);
			break;
		case WindowBackendType::GroupBox:
			gtk_frame_set_label(GTK_FRAME(this->gtkWidget), gtkText);
			break;
		case WindowBackendType::Label:
			gtk_label_set_text(GTK_LABEL(this->gtkWidget), gtkText);
			break;
		case WindowBackendType::PushButton:
			gtk_button_set_label(GTK_BUTTON(this->gtkWidget), gtkText);
			break;
		case WindowBackendType::RadioButton:
			gtk_button_set_label(GTK_BUTTON(this->gtkWidget), gtkText);
			break;
		case WindowBackendType::Window:
			gtk_window_set_title(GTK_WINDOW(this->gtkWidget), gtkText);
			gtk_header_bar_set_title(GTK_HEADER_BAR(this->headerBar), gtkText);
			break;
	}
}

void GtkWindowBackend::SetValue(int32 value)
{
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(this->gtkWidget), value);
}

void GtkWindowBackend::Show(bool visible)
{
	if(this->type == WindowBackendType::Window)
	{
		Size size = this->widget->GetSizeHint();

		//add titlebar
		int min, nat;
		gtk_widget_get_preferred_height(this->headerBar, &min, &nat);
		size.height += Math::Max(min, nat);

		gtk_window_set_default_size(GTK_WINDOW(this->gtkWidget), size.width, size.height);
	}

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
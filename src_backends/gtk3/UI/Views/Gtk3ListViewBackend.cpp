/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
#include "Gtk3ListViewBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Local functions
static gboolean ButtonSlot(GtkWidget* gtkWidget, GdkEvent* event, gpointer user_data)
{
    Gtk3ListViewBackend* backend = static_cast<Gtk3ListViewBackend *>(user_data);
    Widget& widget = backend->GetEditableWidget();

    MouseButton button;
    switch(event->button.button)
    {
        case 1:
            button = MouseButton::Left;
            break;
        case 3:
            button = MouseButton::Right;
            break;
        default:
            return FALSE;
    }

    switch(event->button.type)
    {
        case GDK_BUTTON_PRESS:
        case GDK_BUTTON_RELEASE:
        {
            bool isDown = event->type == GDK_BUTTON_PRESS;
            Math::PointD point = {event->button.x, widget.GetSize().height - event->button.y};
            KeyboardModifiers keyboardModifiers;

            MouseClickEvent mce(button, isDown, point, keyboardModifiers);
            widget.Event(mce);

            return mce.WasAccepted();
        }
    }

    return FALSE;
}

static void RemoveChildCallback(GtkWidget* gtkWidget, gpointer)
{
    gtk_widget_destroy(gtkWidget);
}

static void SelectionChangedSlot(GtkListBox* box, gpointer    user_data)
{
    ListView& listView = *(ListView *) user_data;
    auto controller = listView.GetController();

    LinkedList<ControllerIndex> selection;
    GList* list = gtk_list_box_get_selected_rows(box);

    GList* iter = list;
    while(iter)
    {
        GtkWidget* row = static_cast<GtkWidget *>(iter->data);
        gint index = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(row));
        iter = iter->next;

        selection.InsertTail(controller->GetChildIndex(index, Unsigned<uint32>::Max()));
    }

    g_list_free (list);

    SelectionChangedEvent event(Move(selection));
    listView.Event(event);
}

//Constructor
Gtk3ListViewBackend::Gtk3ListViewBackend(UIBackend &uiBackend, ListView &listView)
    : ViewBackend(uiBackend), WidgetBackend(uiBackend),
    Gtk3WidgetBackend(uiBackend, gtk_scrolled_window_new(nullptr, nullptr)), listView(listView)
{
    GtkWidget* gtkWidget = this->GetGtkWidget();

    this->listBoxWidget = gtk_list_box_new();
    gtk_widget_show(this->listBoxWidget);
    gtk_container_add(GTK_CONTAINER(gtkWidget), this->listBoxWidget);

    g_signal_connect(this->listBoxWidget, "button-press-event", G_CALLBACK(ButtonSlot), this);
    g_signal_connect(this->listBoxWidget, "selected-rows-changed", G_CALLBACK(SelectionChangedSlot), &listView);
}

//Public methods
void Gtk3ListViewBackend::ControllerChanged()
{
    gtk_container_foreach(GTK_CONTAINER(this->listBoxWidget), RemoveChildCallback, nullptr);

    if(this->listView.HasController())
    {
        const auto& controller = *this->listView.GetController();
        uint32 nChildren = controller.GetNumberOfChildren();

        GtkListBox* gtkListBox = GTK_LIST_BOX(this->listBoxWidget);
        for(uint32 i = 0; i < nChildren; i++)
        {
            ControllerIndex childIndex = controller.GetChildIndex(i, 0);
            String text = controller.GetText(childIndex);

            GtkWidget* row = gtk_label_new((const gchar*)text.ToUTF8().GetRawZeroTerminatedData());
            gtk_list_box_insert(gtkListBox, row, -1);
            gtk_widget_show(row);
        }
    }
}

const Widget &Gtk3ListViewBackend::GetWidget() const
{
    return this->listView;
}

void Gtk3ListViewBackend::UpdateSelection() const
{
    GtkListBox* gtkListBox = GTK_LIST_BOX(this->listBoxWidget);
    gtk_list_box_unselect_all(gtkListBox);

    const auto& indexes = this->listView.SelectionController().GetSelectedIndexes();
    for(const auto& index : indexes)
    {
        GtkListBoxRow* row = gtk_list_box_get_row_at_index(gtkListBox, index.GetRow());
        gtk_list_box_select_row(gtkListBox, row);
    }
}

//NOT IMPLEMENTED
void Gtk3ListViewBackend::Repaint()
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
//NOT IMPLEMENTED
/*
 * Copyright (c) 2019-2021 Amir Czwink (amir130@hotmail.de)
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
#include "Gtk3WidgetBackend.hpp"
//Local
#include <Std++/UI/Events/WidgetBoundsChangedEvent.hpp>
#include <Std++/UI/Widget.hpp>
#include <Std++/UI/WidgetContainer.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Local functions
static void SizeAllocateSlot(GtkWidget* gtkWidget, GdkRectangle *allocation, gpointer user_data)
{
	Gtk3WidgetBackend* backend = static_cast<Gtk3WidgetBackend *>(user_data);
	backend->OnSetAllocation(*allocation);
}

//Constructor
Gtk3WidgetBackend::Gtk3WidgetBackend(StdXX::UIBackend &uiBackend, GtkWidget *gtkWidget)
        : WidgetBackend(uiBackend), gtkWidget(gtkWidget)
{
	/*
    if(!GTK_IS_WINDOW(gtkWidget))
        gtk_widget_show(gtkWidget); //default to show
        */

    g_object_set_data(G_OBJECT(gtkWidget), "Std++", this);

	g_signal_connect(gtkWidget, "size-allocate", G_CALLBACK(SizeAllocateSlot), this);
}

//Public methods
Math::RectD Gtk3WidgetBackend::GetNextAssignmentBoundsInGtkCoords() const
{
	Math::RectD bounds = this->nextBounds;

	const Widget& widget = this->GetWidget();
	const WidgetContainer* parent = widget._GetParentWithBackend();
	
	//offset bounds because the parent of the gtk3 widget might not be the parent of the std++ widget
	bounds.origin = widget.TranslateToAncestorCoords(bounds.origin - widget.GetBounds().origin, parent);
	bounds.y() = parent->GetSize().height - bounds.GetVerticalEnd(); //invert "y"-axis for gtk
	
	return bounds;
}

Math::SizeD Gtk3WidgetBackend::GetSizeHint() const
{
	int min1, nat1, min2, nat2;

	gtk_widget_get_preferred_width(this->gtkWidget, &min1, &nat1);
	gtk_widget_get_preferred_height(this->gtkWidget, &min2, &nat2);

	return Math::SizeD(nat1, nat2);
}

void Gtk3WidgetBackend::OnSetAllocation(const GtkAllocation &allocation)
{
	const WidgetContainer* parent = this->GetWidget()._GetParentWithBackend();

	Math::RectD newBounds(allocation.x, allocation.y, allocation.width, allocation.height);

	GtkWidget* gtkParent = gtk_widget_get_parent(this->gtkWidget);
	if(gtkParent)
	{
		GtkAllocation parentAllocation;
		gtk_widget_get_allocation(gtkParent, &parentAllocation);
		newBounds.x() -= parentAllocation.x;
		newBounds.y() -= parentAllocation.y;
	}

	if(parent)
		newBounds.y() = parent->GetSize().height - newBounds.GetVerticalEnd(); //invert "y"-axis from gtk-coords

	WidgetBoundsChangedEvent event(newBounds);
	this->GetEditableWidget().Event(event);
}

void Gtk3WidgetBackend::SetBounds(const Math::RectD& bounds)
{
	this->nextBounds = bounds;
	gtk_widget_queue_resize(this->gtkWidget);
}

void Gtk3WidgetBackend::SetEnabled(bool enable)
{
    gtk_widget_set_sensitive(this->gtkWidget, enable);
}

void Gtk3WidgetBackend::SetHint(const String &text)
{
    gtk_widget_set_tooltip_text(this->gtkWidget, reinterpret_cast<const gchar *>(text.ToUTF8().GetRawZeroTerminatedData()));
}

void Gtk3WidgetBackend::Show(bool visible)
{
    if(visible)
        gtk_widget_show(this->gtkWidget);
    else
        gtk_widget_hide(this->gtkWidget);
}
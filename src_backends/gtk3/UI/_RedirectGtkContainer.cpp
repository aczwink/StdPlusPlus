/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
//Corresponding header
#include "_RedirectGtkContainer.h"
//Global
#include <gtk/gtk.h>
#include <math.h>
//Local
#include <Std++/UI/Containers/CompositeWidget.hpp>
#include "Gtk3WidgetBackend.hpp"

#define REDIRECT_CONTAINER_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), REDIRECT_CONTAINER_TYPE, RedirectContainerPrivate))
typedef struct _RedirectContainerPrivate RedirectContainerPrivate;
struct _RedirectContainerPrivate
{
	GList *firstChild;
};

//Move declarations of the "methods"
static void redirect_container_add(GtkContainer *container, GtkWidget *widget);
static GType redirect_container_child_type(GtkContainer *container);
static void redirect_container_forall(GtkContainer *container, gboolean include_internals, GtkCallback callback, gpointer callback_data);
static void redirect_container_get_preferred_height(GtkWidget *redirContainer, int *minimal, int *natural);
static void redirect_container_get_preferred_width(GtkWidget *widget, int *minimal, int *natural);
static void redirect_container_remove(GtkContainer *container, GtkWidget *widget);
static void redirect_container_size_allocate(GtkWidget *redirContainer, GtkAllocation *allocation);


//Define the "class" RedirectContainer as subclass from GtkContainer
G_DEFINE_TYPE(RedirectContainer, redirect_container, GTK_TYPE_CONTAINER);

static void redirect_container_class_init(RedirectContainerClass *klass)
{
	//Override GtkWidget methods
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
	widget_class->get_preferred_width = redirect_container_get_preferred_width;
	widget_class->get_preferred_height = redirect_container_get_preferred_height;
	widget_class->size_allocate = redirect_container_size_allocate;

	//Override GtkContainer methods
	GtkContainerClass *container_class = GTK_CONTAINER_CLASS(klass);
	container_class->add = redirect_container_add;
	container_class->child_type = redirect_container_child_type;
	container_class->forall = redirect_container_forall;
	container_class->remove = redirect_container_remove;

	g_type_class_add_private(klass, sizeof(RedirectContainerPrivate));
}

GtkWidget *redirect_container_new()
{
	return GTK_WIDGET(g_object_new(redirect_container_get_type(), NULL));
}





//Now define the "methods"
static void redirect_container_init(RedirectContainer *square)
{
	gtk_widget_set_has_window(GTK_WIDGET(square), FALSE); //does not have a GDK window (like X11 and so on)

	RedirectContainerPrivate *priv = REDIRECT_CONTAINER_PRIVATE(square);
	priv->firstChild = NULL;
}

static void redirect_container_add(GtkContainer *container, GtkWidget *widget)
{
	g_return_if_fail(container || IS_REDIRECT_CONTAINER(container));
	g_return_if_fail(widget || GTK_IS_WIDGET(widget));
	g_return_if_fail(gtk_widget_get_parent(widget) == NULL);

	RedirectContainerPrivate *priv = REDIRECT_CONTAINER_PRIVATE(container);

	ASSERT(widget, "If you see this, report to StdPlusPlus");

	priv->firstChild = g_list_append(priv->firstChild, widget);
	gtk_widget_set_parent(widget, GTK_WIDGET(container));

	if(gtk_widget_get_visible(widget))
		gtk_widget_queue_resize(GTK_WIDGET(container));
}

static GType redirect_container_child_type(GtkContainer *container)
{
	return GTK_TYPE_WIDGET; //we accept widgets as children
}

static void redirect_container_forall(GtkContainer *container, gboolean include_internals, GtkCallback callback, gpointer callback_data)
{
	RedirectContainerPrivate *priv = REDIRECT_CONTAINER_PRIVATE(container);

	g_list_foreach(priv->firstChild, (GFunc)callback, callback_data);
}

static void redirect_container_get_preferred_height(GtkWidget *redirContainer, int *minimal, int *natural)
{
	g_return_if_fail(redirContainer != NULL);
	g_return_if_fail(IS_REDIRECT_CONTAINER(redirContainer));

	_stdxx_::Gtk3WidgetBackend* container = _stdxx_::Gtk3WidgetBackend::Gtk3WidgetBackendFromGtkWidget(redirContainer);

	*minimal = container->GetWidget().GetSizeHint().height;
	*natural = *minimal;
}

static void redirect_container_get_preferred_width(GtkWidget *redirContainer, int *minimal, int *natural)
{
	g_return_if_fail(redirContainer != NULL);
	g_return_if_fail(IS_REDIRECT_CONTAINER(redirContainer));

    _stdxx_::Gtk3WidgetBackend* container = _stdxx_::Gtk3WidgetBackend::Gtk3WidgetBackendFromGtkWidget(redirContainer);

	*minimal = container->GetWidget().GetSizeHint().width;
	*natural = *minimal;
}

static void redirect_container_remove(GtkContainer *container, GtkWidget *widget)
{
	g_return_if_fail(container || IS_REDIRECT_CONTAINER(container));
	g_return_if_fail(widget || GTK_IS_WIDGET(widget));

	RedirectContainerPrivate *priv = REDIRECT_CONTAINER_PRIVATE(container);

	GList *link = g_list_find(priv->firstChild, widget);
	if(link) {
		gboolean was_visible = gtk_widget_get_visible(widget);
		gtk_widget_unparent(widget);

		priv->firstChild = g_list_delete_link(priv->firstChild, link);

		if(was_visible)
			gtk_widget_queue_resize(GTK_WIDGET(container));
	}
}

static void redirect_container_size_allocate(GtkWidget *redirContainer, GtkAllocation *allocation)
{
	g_return_if_fail(redirContainer != NULL || allocation != NULL);
	g_return_if_fail(IS_REDIRECT_CONTAINER(redirContainer));

	RedirectContainerPrivate *priv = REDIRECT_CONTAINER_PRIVATE(redirContainer);
    _stdxx_::Gtk3WidgetBackend* thisContainer = _stdxx_::Gtk3WidgetBackend::Gtk3WidgetBackendFromGtkWidget(redirContainer);

	gtk_widget_set_allocation(redirContainer, allocation);
	const int containerHeight = allocation->height;

	for(GList *iter = priv->firstChild; iter; iter = iter->next)
	{
		GtkWidget *gtkWidget = (GtkWidget *) iter->data;

		{
			int tmp;
			gtk_widget_get_preferred_width(gtkWidget, &tmp, &tmp); //make gtk3 happy, else it will print out warnings -.-
		}

        _stdxx_::Gtk3WidgetBackend* widgetBackend = _stdxx_::Gtk3WidgetBackend::Gtk3WidgetBackendFromGtkWidget(gtkWidget);
		if(!widgetBackend)
			continue;
        StdXX::UI::Widget &widget = widgetBackend->GetWidget();
        StdXX::Math::RectD bounds = widget.GetLocalBounds();

		//offset bounds because the parent of the gtk3 widget might not be the parent of the std++ widget
		bounds.origin = widget.TranslateToAncestorCoords(bounds.origin, (const StdXX::UI::WidgetContainer*)&thisContainer->GetWidget());
		bounds.y() = containerHeight - bounds.GetVerticalEnd(); //invert "y"-axis for gtk

		allocation->x = bounds.x();
		allocation->y = bounds.y();
		allocation->width = bounds.width();
		allocation->height = bounds.height();
		//fprintf(stderr, "size-allocate: %p -> %g, %g, %g, %g\n", &widget, bounds.x(), bounds.y(), bounds.width(), bounds.height());
		//fflush(stderr);
		//if(gtk_widget_get_realized(gtkWidget))
			gtk_widget_size_allocate(gtkWidget, allocation);
	}
}
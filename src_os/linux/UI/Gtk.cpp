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
//corresponding header
#include "Gtk.h"
//Local
#include "_RedirectGtkContainer.h"

//Global functions
_AC_Gtk_WidgetPrivate *CreateWidgetPrivateData(GtkWidget *gtkWidget, Widget *widget)
{
	_AC_Gtk_WidgetPrivate *priv;

	priv = (_AC_Gtk_WidgetPrivate *)MemAlloc(sizeof(_AC_Gtk_WidgetPrivate));

	priv->widget = gtkWidget;
	priv->childAreaWidget = nullptr;

	g_object_set_data(G_OBJECT(priv->widget), "ACStdLib", widget);

	return priv;
}

_AC_Gtk_WidgetPrivate *CreateWidgetContainerPrivateData(GtkWidget *gtkWidget, Widget *widget)
{
	_AC_Gtk_WidgetPrivate *priv = CreateWidgetPrivateData(gtkWidget, widget);

	//child area
	priv->childAreaWidget = redirect_container_new();

	g_object_set_data(G_OBJECT(priv->childAreaWidget), "ACStdLib", widget);

	gtk_widget_show(priv->childAreaWidget); //default to show
	gtk_container_add(GTK_CONTAINER(priv->widget), priv->childAreaWidget);

	return priv;
}

void DestroyWidgetPrivateData(_AC_Gtk_WidgetPrivate *priv)
{
	if(priv->childAreaWidget)
		gtk_widget_destroy(GTK_WIDGET(priv->childAreaWidget));

	gtk_widget_destroy(GTK_WIDGET(priv->widget));

	MemFree(priv);
}

Size GetPreferedSizeGtk(GtkWidget *widget)
{
	int min1, nat1, min2, nat2;

	gtk_widget_get_preferred_width(widget, &min1, &nat1);
	gtk_widget_get_preferred_height(widget, &min2, &nat2);

	return Size(nat1, nat2);
}
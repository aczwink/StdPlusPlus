/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/UI/Widget.hpp>
//Global
#include <gtk/gtk.h>
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

struct _AC_Gtk_Menu
{
    GtkWidget *pItem;
    GtkWidget *pSubMenu;
};

struct _AC_Gtk_WidgetPrivate
{
    GtkWidget *widget; //the widget, window whatever itself
    GtkWidget *childAreaWidget; //its child area
};

//Definitions
#define PRIVATE_DATA(widget) ((_AC_Gtk_WidgetPrivate *)(widget)->_GetBackend())
#define WIDGET_FROM_GTK(gtkWidget) ((Widget *)g_object_get_data(G_OBJECT(gtkWidget), u8"Std++"))
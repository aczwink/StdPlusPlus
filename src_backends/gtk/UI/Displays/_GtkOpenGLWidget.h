/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define AC_GTK_OPENGL_WIDGET_TYPE                (ac_gtk_opengl_widget_get_type ())
#define AC_GTK_OPENGL_WIDGET(obj)                (G_TYPE_CHECK_INSTANCE_CAST ((obj), AC_GTK_OPENGL_WIDGET_TYPE, AC_Gtk_OpenGLWidget))
#define IS_AC_GTK_OPENGL_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AC_GTK_OPENGL_WIDGET_TYPE))

typedef struct _AC_Gtk_OpenGLWidget AC_Gtk_OpenGLWidget;
typedef struct _AC_Gtk_OpenGLWidgetClass AC_Gtk_OpenGLWidgetClass;

struct _AC_Gtk_OpenGLWidget
{
	GtkWidget parent_instance;
};

struct _AC_Gtk_OpenGLWidgetClass
{
	GtkWidgetClass parent_class;
};

//Functions
GType ac_gtk_opengl_widget_get_type(void) G_GNUC_CONST;
GtkWidget *ac_gtk_opengl_widget_new(void);
GdkGLContext *ac_gtk_opengl_widget_get_context(AC_Gtk_OpenGLWidget *area);
void ac_gtk_opengl_widget_setwidget(AC_Gtk_OpenGLWidget *openGLWidget, void *widget);

G_END_DECLS
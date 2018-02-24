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
#pragma once
#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define REDIRECT_CONTAINER_TYPE            (redirect_container_get_type())
#define REDIRECT_CONTAINER(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), REDIRECT_CONTAINER_TYPE, RedirectContainer))
#define REDIRECT_CONTAINER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), REDIRECT_CONTAINER_TYPE, RedirectContainerClass))
#define IS_REDIRECT_CONTAINER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), REDIRECT_CONTAINER_TYPE))
#define IS_REDIRECT_CONTAINER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), REDIRECT_CONTAINER_TYPE))

typedef struct _RedirectContainer RedirectContainer;
typedef struct _RedirectContainerClass  RedirectContainerClass;

struct _RedirectContainer
{
	GtkContainer parent_instance;
};

struct _RedirectContainerClass
{
	GtkContainerClass parent_class;
};

GType redirect_container_get_type(void) G_GNUC_CONST;
GtkWidget *redirect_container_new(void);

G_END_DECLS
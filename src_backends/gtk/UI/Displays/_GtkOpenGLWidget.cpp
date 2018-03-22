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
//Corresponding header
#include "_GtkOpenGLWidget.h"
//Local
#include "../../../OpenGL/GLFunctions.h"
#include "../GtkEventQueue.hpp"

typedef struct {
	StdPlusPlus::UI::Widget *widget;

	GdkGLContext *context;

	gboolean have_buffers;

	guint frame_buffer;
	guint texture;
	guint depth_stencil_buffer;

	gboolean resized;
} AC_Gtk_OpenGLWidgetPrivate;

//"Class" definition
G_DEFINE_TYPE_WITH_PRIVATE (AC_Gtk_OpenGLWidget, ac_gtk_opengl_widget, GTK_TYPE_WIDGET)

//Local functions
static GdkGLContext *create_context(AC_Gtk_OpenGLWidget *area)
{
	AC_Gtk_OpenGLWidgetPrivate *priv = (AC_Gtk_OpenGLWidgetPrivate *)ac_gtk_opengl_widget_get_instance_private (area);
	GtkWidget *widget = GTK_WIDGET (area);
	GError *error = NULL;
	GdkGLContext *context;

	context = gdk_window_create_gl_context (gtk_widget_get_window (widget), &error);
	if (error != NULL)
	{
		g_clear_object (&context);
		g_clear_error (&error);
		return NULL;
	}

	gdk_gl_context_set_use_es(context, FALSE);
	gdk_gl_context_set_required_version(context, 3, 3);

	gdk_gl_context_realize (context, &error);
	if (error != NULL)
	{
		g_clear_object (&context);
		g_clear_error (&error);
		return NULL;
	}

	return context;
}

static void ac_gtk_opengl_widget_allocate_buffers(AC_Gtk_OpenGLWidget *area)
{
	AC_Gtk_OpenGLWidgetPrivate *priv = (AC_Gtk_OpenGLWidgetPrivate *)ac_gtk_opengl_widget_get_instance_private (area);
	GtkWidget *widget = GTK_WIDGET (area);
	int scale, width, height;

	if (priv->context == NULL)
		return;

	scale = gtk_widget_get_scale_factor (widget);
	width = gtk_widget_get_allocated_width (widget) * scale;
	height = gtk_widget_get_allocated_height (widget) * scale;

	glBindTexture (GL_TEXTURE_2D, priv->texture);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

	glBindRenderbuffer(GL_RENDERBUFFER, priv->depth_stencil_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

static void ac_gtk_opengl_widget_ensure_buffers(AC_Gtk_OpenGLWidget *area)
{
	AC_Gtk_OpenGLWidgetPrivate *priv = (AC_Gtk_OpenGLWidgetPrivate *)ac_gtk_opengl_widget_get_instance_private (area);
	GtkWidget *widget = GTK_WIDGET (area);

	gtk_widget_realize (widget);

	if (priv->context == NULL)
		return;

	if (priv->have_buffers)
		return;

	priv->have_buffers = TRUE;

	glGenFramebuffers (1, &priv->frame_buffer);
	if (priv->texture == 0)
		glGenTextures (1, &priv->texture);

	if (priv->depth_stencil_buffer == 0)
		glGenRenderbuffers (1, &priv->depth_stencil_buffer);

	ac_gtk_opengl_widget_allocate_buffers(area);
}

static void ac_gtk_opengl_widget_attach_buffers(AC_Gtk_OpenGLWidget *area)
{
	AC_Gtk_OpenGLWidgetPrivate *priv = (AC_Gtk_OpenGLWidgetPrivate *)ac_gtk_opengl_widget_get_instance_private (area);

	g_return_if_fail (IS_AC_GTK_OPENGL_WIDGET (area));

	if (priv->context == NULL)
		return;

	gdk_gl_context_make_current(ac_gtk_opengl_widget_get_context(area));

	if (!priv->have_buffers)
		ac_gtk_opengl_widget_ensure_buffers(area);
	else if (priv->resized)
	{
		priv->resized = FALSE;
		ac_gtk_opengl_widget_allocate_buffers(area);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, priv->frame_buffer);

	glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, priv->texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, priv->depth_stencil_buffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, priv->depth_stencil_buffer);
}

static void ac_gtk_opengl_widget_delete_buffers(AC_Gtk_OpenGLWidget *area)
{
	AC_Gtk_OpenGLWidgetPrivate *priv = (AC_Gtk_OpenGLWidgetPrivate *)ac_gtk_opengl_widget_get_instance_private (area);

	if (priv->context == NULL)
		return;

	priv->have_buffers = FALSE;

	if (priv->texture != 0)
	{
		glDeleteTextures(1, &priv->texture);
		priv->texture = 0;
	}

	if (priv->depth_stencil_buffer != 0)
	{
		glDeleteRenderbuffers (1, &priv->depth_stencil_buffer);
		priv->depth_stencil_buffer = 0;
	}

	if (priv->frame_buffer != 0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &priv->frame_buffer);
		priv->frame_buffer = 0;
	}
}

//"Constructor"
static void ac_gtk_opengl_widget_init(AC_Gtk_OpenGLWidget *area)
{
	AC_Gtk_OpenGLWidgetPrivate *priv = (AC_Gtk_OpenGLWidgetPrivate *)ac_gtk_opengl_widget_get_instance_private (area);

	gtk_widget_set_has_window(GTK_WIDGET (area), FALSE);
}

//"Destructor"
static void ac_gtk_opengl_widget_dispose(GObject *gobject)
{
	AC_Gtk_OpenGLWidget *area = AC_GTK_OPENGL_WIDGET (gobject);
	AC_Gtk_OpenGLWidgetPrivate *priv = (AC_Gtk_OpenGLWidgetPrivate *) ac_gtk_opengl_widget_get_instance_private (area);

	g_clear_object (&priv->context);

	G_OBJECT_CLASS (ac_gtk_opengl_widget_parent_class)->dispose(gobject);
}

//"Members"
static void ac_gtk_opengl_widget_realize (GtkWidget *widget)
{
	AC_Gtk_OpenGLWidget *area = AC_GTK_OPENGL_WIDGET (widget);
	AC_Gtk_OpenGLWidgetPrivate *priv = (AC_Gtk_OpenGLWidgetPrivate *)ac_gtk_opengl_widget_get_instance_private (area);

	GTK_WIDGET_CLASS(ac_gtk_opengl_widget_parent_class)->realize(widget);

	priv->context = create_context(area);

	gdk_gl_context_make_current(ac_gtk_opengl_widget_get_context(area));
	priv->resized = TRUE;
	priv->have_buffers = FALSE;
}

static void ac_gtk_opengl_widget_unrealize (GtkWidget *widget)
{
	AC_Gtk_OpenGLWidget *area = AC_GTK_OPENGL_WIDGET (widget);
	AC_Gtk_OpenGLWidgetPrivate *priv = (AC_Gtk_OpenGLWidgetPrivate *)ac_gtk_opengl_widget_get_instance_private (area);

	if (priv->context != NULL)
	{
		if (priv->have_buffers)
		{
			gdk_gl_context_make_current(ac_gtk_opengl_widget_get_context(area));
			ac_gtk_opengl_widget_delete_buffers(area);
		}

		/* Make sure to unset the context if current */
		if (priv->context == gdk_gl_context_get_current ())
			gdk_gl_context_clear_current ();
	}

	g_clear_object (&priv->context);

	GTK_WIDGET_CLASS(ac_gtk_opengl_widget_parent_class)->unrealize (widget);
}

static void ac_gtk_opengl_widget_size_allocate (GtkWidget *widget, GtkAllocation *allocation)
{
	AC_Gtk_OpenGLWidget *area = AC_GTK_OPENGL_WIDGET (widget);
	AC_Gtk_OpenGLWidgetPrivate *priv = (AC_Gtk_OpenGLWidgetPrivate *)ac_gtk_opengl_widget_get_instance_private (area);

	GTK_WIDGET_CLASS (ac_gtk_opengl_widget_parent_class)->size_allocate (widget, allocation);

	if (gtk_widget_get_realized (widget))
		priv->resized = TRUE;
}

static gboolean ac_gtk_opengl_widget_draw(GtkWidget *widget, cairo_t *cr)
{
	AC_Gtk_OpenGLWidget *area = AC_GTK_OPENGL_WIDGET (widget);
	AC_Gtk_OpenGLWidgetPrivate *priv = (AC_Gtk_OpenGLWidgetPrivate *)ac_gtk_opengl_widget_get_instance_private (area);
	int w, h, scale;

	if (priv->context == NULL)
		return FALSE;

	gdk_gl_context_make_current(ac_gtk_opengl_widget_get_context(area));

	ac_gtk_opengl_widget_attach_buffers(area);

	scale = gtk_widget_get_scale_factor (widget);
	w = gtk_widget_get_allocated_width (widget) * scale;
	h = gtk_widget_get_allocated_height (widget) * scale;

	GLenum status = glCheckFramebufferStatus (GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_COMPLETE)
	{
		GtkEventQueue::OpenGLWidgetRender(priv->widget);

		gdk_cairo_draw_from_gl(cr, gtk_widget_get_window (widget), priv->texture, GL_TEXTURE, scale, 0, 0, w, h);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		gdk_gl_context_clear_current();
	}
	else
	{
		g_warning ("fb setup not supported");
	}

	return TRUE;
}

//Building "vtable"
static void ac_gtk_opengl_widget_class_init (AC_Gtk_OpenGLWidgetClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

	gobject_class->dispose = ac_gtk_opengl_widget_dispose;

	widget_class->realize = ac_gtk_opengl_widget_realize;
	widget_class->unrealize = ac_gtk_opengl_widget_unrealize;
	widget_class->size_allocate = ac_gtk_opengl_widget_size_allocate;
	widget_class->draw = ac_gtk_opengl_widget_draw;

	gtk_widget_class_set_accessible_role (widget_class, ATK_ROLE_DRAWING_AREA);
}

//Functions
GtkWidget *ac_gtk_opengl_widget_new(void)
{
	return GTK_WIDGET(g_object_new (AC_GTK_OPENGL_WIDGET_TYPE, NULL));
}

GdkGLContext *ac_gtk_opengl_widget_get_context(AC_Gtk_OpenGLWidget *area)
{
	AC_Gtk_OpenGLWidgetPrivate *priv = (AC_Gtk_OpenGLWidgetPrivate *)ac_gtk_opengl_widget_get_instance_private (area);

	g_return_val_if_fail (IS_AC_GTK_OPENGL_WIDGET (area), NULL);

	return priv->context;
}

void ac_gtk_opengl_widget_setwidget(AC_Gtk_OpenGLWidget *openGLWidget, void *widget)
{
	AC_Gtk_OpenGLWidgetPrivate *priv = (AC_Gtk_OpenGLWidgetPrivate *)ac_gtk_opengl_widget_get_instance_private(openGLWidget);

	priv->widget = (StdPlusPlus::UI::Widget *) widget;
}
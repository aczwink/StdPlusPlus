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
//Class header
#include <ACStdLib/UI/Displays/RenderTargetWidget.hpp>
//Local
#include "../Gtk.h"
#include "../GtkEventQueue.hpp"
#include "_GtkOpenGLWidget.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;
//Definitions
#define THIS (PRIVATE_DATA(this)->widget)


//Local functions
static void OnRealize(GtkWidget *gtkWidget, gpointer user_data)
{
	GtkGLArea *glArea = GTK_GL_AREA(gtkWidget);

	gtk_gl_area_make_current(glArea);

	//enable depth buffer
	gtk_gl_area_set_has_depth_buffer(glArea, true);
	//glDisable(GL_DEPTH_TEST); //stupid gtk!!! GL_DEPTH_TEST is disabled by default even if the def framebuffer has a depth buffer
}

//Private methods
void RenderTargetWidget::CreateOSHandle()
{
	//TODO: own gtk gl area because: GL_DEPTH_TEST is enabled by gtk, when a frame buffer has a depth buffer-.-
    //this->backend = CreateWidgetPrivateData(ac_gtk_opengl_widget_new(), this);
	//ac_gtk_opengl_widget_setwidget(AC_GTK_OPENGL_WIDGET(THIS), this);

	NOT_IMPLEMENTED_ERROR; //TODO: new implementation
	//this->backend = CreateWidgetPrivateData(gtk_gl_area_new(), this);
	g_signal_connect(THIS, "realize", G_CALLBACK(OnRealize), this);
	g_signal_connect(THIS, "render", G_CALLBACK(GtkEventQueue::PaintSlot), this);

	//ADD_SELF_TO_PARENT(THIS);
	gtk_widget_show(THIS);

	gtk_widget_add_events(THIS, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK | GDK_SCROLL_MASK);
	g_signal_connect(THIS, "button-press-event", G_CALLBACK(GtkEventQueue::ButtonSlot), this);
	g_signal_connect(THIS, "button-release-event", G_CALLBACK(GtkEventQueue::ButtonSlot), this);
	g_signal_connect(THIS, "motion-notify-event", G_CALLBACK(GtkEventQueue::MouseMotionSlot), this);
	g_signal_connect(THIS, "scroll-event", G_CALLBACK(GtkEventQueue::ScrollSlot), this);

	gtk_widget_realize(THIS); //important!!!
	gtk_gl_area_attach_buffers(GTK_GL_AREA(THIS)); //also important so that device context can read default frame buffer
}

void RenderTargetWidget::System_Destroy()
{
	MemFree(this->backend);
}
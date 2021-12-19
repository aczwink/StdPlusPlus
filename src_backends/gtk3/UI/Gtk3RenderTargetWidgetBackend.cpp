/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#include "Gtk3RenderTargetWidgetBackend.hpp"
//Local
#include "../Rendering/GtkOpenGLDeviceContext.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

static bool RenderSlot(GtkGLArea *glArea, GdkGLContext *context, gpointer user_data)
{
	RenderTargetWidget& renderTargetWidget = *(RenderTargetWidget*)user_data;

	GtkOpenGLDeviceContext& gtkOpenGlDeviceContext = (GtkOpenGLDeviceContext &) renderTargetWidget.DeviceContext();
	gtkOpenGlDeviceContext.ResetDepthTest();

	GtkAllocation gtkAllocation;
	gtk_widget_get_allocation(GTK_WIDGET(glArea), &gtkAllocation);

	PaintEvent paintEvent({ static_cast<float64>(gtkAllocation.x), static_cast<float64>(gtkAllocation.y),
						 static_cast<float64>(gtkAllocation.width), static_cast<float64>(gtkAllocation.height) });
	renderTargetWidget.Event(paintEvent);

	return paintEvent.WasAccepted();
}

//Constructor
Gtk3RenderTargetWidgetBackend::Gtk3RenderTargetWidgetBackend(UIBackend &uiBackend, RenderTargetWidget& renderTargetWidget)
	: Gtk3WidgetBackend(uiBackend, gtk_gl_area_new()), WidgetBackend(uiBackend),
	renderTargetWidget(renderTargetWidget)
{
	gtk_gl_area_set_has_depth_buffer(GTK_GL_AREA(this->GetGtkWidget()), true);

	g_signal_connect(this->GetGtkWidget(), "render", G_CALLBACK(RenderSlot), &renderTargetWidget);
}

//Public methods
const UI::Widget &Gtk3RenderTargetWidgetBackend::GetWidget() const
{
	return this->renderTargetWidget;
}

void Gtk3RenderTargetWidgetBackend::Repaint()
{
	gtk_widget_queue_draw(this->GetGtkWidget());
}
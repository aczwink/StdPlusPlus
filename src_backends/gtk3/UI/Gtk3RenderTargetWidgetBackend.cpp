/*
 * Copyright (c) 2020-2024 Amir Czwink (amir130@hotmail.de)
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

//Local functions
static gboolean ButtonSlot(GtkWidget* gtkWidget, GdkEvent* event, gpointer user_data)
{
	RenderTargetWidget& renderTargetWidget = *(RenderTargetWidget*)user_data;

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
			bool isDown = event->button.type == GDK_BUTTON_PRESS;
			Math::PointD point = {event->button.x, renderTargetWidget.GetSize().height - event->button.y};
			KeyboardModifiers keyboardModifiers;

			MouseClickEvent mce(button, isDown, point, keyboardModifiers);
			renderTargetWidget.Event(mce);

			return mce.WasAccepted();
		}
	}

	return FALSE;
}

static bool MouseMotionSlot(GtkWidget *gtkWidget, GdkEventMotion *event, gpointer user_data)
{
	RenderTargetWidget& renderTargetWidget = *(RenderTargetWidget*)user_data;

	Math::PointD pos = {event->x, ((float64)renderTargetWidget.GetSize().height) - event->y};
	KeyboardModifiers keyboardModifiers;
	MouseEvent mouseEvent(EventType::MouseMoved, pos, keyboardModifiers);

	renderTargetWidget.Event(mouseEvent);

	return mouseEvent.WasAccepted();
}

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

	GtkWidget* gtkWidget = this->GetGtkWidget();
	gtk_widget_add_events(gtkWidget, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK | GDK_SCROLL_MASK);
	g_signal_connect(gtkWidget, "button-press-event", G_CALLBACK(ButtonSlot), &renderTargetWidget);
	g_signal_connect(gtkWidget, "button-release-event", G_CALLBACK(ButtonSlot), &renderTargetWidget);
	g_signal_connect(gtkWidget, "motion-notify-event", G_CALLBACK(MouseMotionSlot), &renderTargetWidget);
	g_signal_connect(gtkWidget, "render", G_CALLBACK(RenderSlot), &renderTargetWidget);
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
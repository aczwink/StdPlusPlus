/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Constructor
Gtk3RenderTargetWidgetBackend::Gtk3RenderTargetWidgetBackend(UIBackend &uiBackend, UI::RenderTargetWidget& renderTargetWidget)
	: Gtk3WidgetBackend(uiBackend, gtk_gl_area_new()), WidgetBackend(uiBackend),
	renderTargetWidget(renderTargetWidget)
{
	gtk_gl_area_set_has_depth_buffer(GTK_GL_AREA(this->GetGtkWidget()), true);
}

//Public methods
UI::Widget &Gtk3RenderTargetWidgetBackend::GetWidget()
{
	return this->renderTargetWidget;
}

const UI::Widget &Gtk3RenderTargetWidgetBackend::GetWidget() const
{
	return this->renderTargetWidget;
}

void Gtk3RenderTargetWidgetBackend::Repaint()
{
	gtk_widget_queue_draw(this->GetGtkWidget());
}




///NOT IMPLEMENTED
void Gtk3RenderTargetWidgetBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
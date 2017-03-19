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
#include "../../../headers/UI/CWindow.h"
//Global
#include <gtk/gtk.h>
//Local
#include "CGtkEventQueue.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Eventhandlers
void CWindow::OnPaint()
{
    NOT_IMPLEMENTED_ERROR;
}

//Private methods
void CWindow::CreateOSWindow(const CRect &refRect)
{
    this->pOSHandle = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    g_object_set_data(G_OBJECT(this->pOSHandle), "ACStdLib", this);

    gtk_window_set_position(GTK_WINDOW(this->pOSHandle), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(this->pOSHandle), refRect.width(), refRect.height());

    g_signal_connect(this->pOSHandle, "delete-event", G_CALLBACK(CGtkEventQueue::CloseSlot), nullptr);
    g_signal_connect(this->pOSHandle, "destroy", G_CALLBACK(CGtkEventQueue::DestroySlot), this);
}

void CWindow::DestroyOSWindow()
{
    gtk_widget_destroy(GTK_WIDGET(this->pOSHandle));
}
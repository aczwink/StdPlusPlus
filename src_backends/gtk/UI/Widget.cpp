/*
 * Copyright (c) 2017-2 Amir Czwink (amir130@hotmail.de)
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
#include <ACStdLib/UI/Widget.hpp>
//Global
#include <gtk/gtk.h>
//Local
#include <ACStdLib/UI/WidgetContainer.hpp>
#include "Gtk.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Global variables
extern bool g_ignoreEvent;

//Proctected methods
void Widget::IgnoreEvent()
{
	g_ignoreEvent = true;
}

//Public methods
void Widget::Repaint()
{
	gtk_widget_queue_draw(PRIVATE_DATA(this)->widget);
}
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
//Global
#include <gtk/gtk.h>
//Local
#include <Std++/UI/EventQueue.hpp>

class GtkEventQueue : public StdPlusPlus::UI::EventQueue
{
public:
    //Functions;
	static bool ButtonSlot(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
	static void ChangedSlot(GtkComboBox *comboBox, gpointer user_data);
	static void CheckResizeSlot(GtkContainer *container, gpointer user_data);
	static void ClickedSlot(GtkButton *button, gpointer user_data);
	static bool CloseSlot(GtkWidget *pWidget, GdkEvent *pEvent);
    static void DestroySlot(GtkWidget *pWidget, gpointer data);
	static bool MouseMotionSlot(GtkWidget *gtkWidget, GdkEventMotion *event, gpointer user_data);
	static bool PaintSlot(GtkGLArea *glArea, GdkGLContext *context, gpointer user_data);
	static bool ScrollSlot(GtkWidget *gtkWidget, GdkEventScroll *event, gpointer user_data);
	static void SizeAllocateSlot(GtkWidget *widget, GdkRectangle *allocation, gpointer user_data);
	static void ToggledSlot(GtkToggleButton *toggleButton, gpointer user_data);
	static void TreeSelectionSlot(GtkTreeSelection *treeSelection, gpointer user_data);

	//Inline
	inline static void OpenGLWidgetRender(StdPlusPlus::UI::Widget *widget)
	{
		StdPlusPlus::UI::EventQueue::DispatchPaintEvent(*widget);
	}
};
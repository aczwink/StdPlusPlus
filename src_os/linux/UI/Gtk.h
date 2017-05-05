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
//Local
#include <ACStdLib/UI/Widget.h>
//Global
#include <gtk/gtk.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

struct _AC_Gtk_Menu
{
    GtkWidget *pItem;
    GtkWidget *pSubMenu;
};

struct _AC_Gtk_WidgetPrivate
{
    GtkWidget *widget; //the widget, window whatever itself
    GtkWidget *childAreaWidget; //its child area
};

class CFullAccessWidget : public Widget
{
public:
	//Constructor
	inline CFullAccessWidget() : Widget(nullptr){} //to make compiler happy

    //Inline
    inline void *GetInternal()
    {
        return this->systemHandle;
    }
};

//Definitions
#define PRIVATE_DATA(widget) ((_AC_Gtk_WidgetPrivate *)((CFullAccessWidget *)widget)->GetInternal())
#define WIDGET_FROM_GTK(gtkWidget) ((Widget *)g_object_get_data(G_OBJECT(gtkWidget), "ACStdLib"))

#define ADD_SELF_TO_PARENT(child) gtk_container_add(GTK_CONTAINER(PRIVATE_DATA(this->GetParent())->childAreaWidget), child);

//Prototypes
_AC_Gtk_WidgetPrivate *CreateWidgetPrivateData(GtkWidget *gtkWidget, Widget *widget);
_AC_Gtk_WidgetPrivate *CreateWidgetContainerPrivateData(GtkWidget *gtkWidget, Widget *widget);
void DestroyWidgetPrivateData(_AC_Gtk_WidgetPrivate *priv);
Size GetPreferedSizeGtk(GtkWidget *widget);
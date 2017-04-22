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
#include <ACStdLib/UI/AWidget.h>
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

struct _AC_Gtk_WidgetContainer
{
    GtkWidget *widget; //the window
    GtkWidget *pChildAreaWidget;
};

class CFullAccessWidget : public AWidget
{
public:
    //Inline
    inline void *GetInternal()
    {
        return this->pOSHandle;
    }
};

#define WIDGET_FROM_GTK_WIDGET(pGtkWidget) (g_object_get_data(G_OBJECT(pGtkWidget), "ACStdLib"))

#define INTERNAL_FROM_WIDGET(pWidget) (((CFullAccessWidget *)pWidget)->GetInternal())
#define INTERNAL_WIDGET_CONTAINER(pWidget) ((_AC_Gtk_WidgetContainer *)INTERNAL_FROM_WIDGET(pWidget))

#define ADD_SELF_TO_PARENT gtk_box_pack_start(GTK_BOX(INTERNAL_WIDGET_CONTAINER(this->GetParent())->pChildAreaWidget), THIS, TRUE, TRUE, 0);
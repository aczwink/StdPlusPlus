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
#include <ACStdLib/UI/Menu/CMenu.h>
//Global
#include <gtk/gtk.h>
//Local
#include <ACStdLib/Containers/Strings/UTF-8/UTF8String.hpp>
#include <ACStdLib/UI/CAction.hpp>
#include "../Gtk.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;
//Definitions
#define THIS ((_AC_Gtk_Menu *)this->pOSHandle)

//Local functions
static void OnMenuItemActivatedHandler(GtkMenuItem *pItem, CActionEntry *pEntry)
{
    if(pEntry->refAction.triggeredEvent)
        pEntry->refAction.triggeredEvent();
}

//Constructor
CMenu::CMenu(const String &refText)
{
    UTF8String name(refText.GetUTF16());

    this->pOSHandle = (_AC_Gtk_Menu *)MemAlloc(sizeof(_AC_Gtk_Menu));

    THIS->pItem = gtk_menu_item_new_with_label((gchar *)name.GetC_Str());
    THIS->pSubMenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(THIS->pItem), THIS->pSubMenu);

    gtk_widget_show(THIS->pItem); //default is show
    gtk_widget_show(THIS->pSubMenu); //default is show
}

//Private methods
void CMenu::AppendItemOS(CActionEntry *pEntry)
{
    GtkWidget *pItem;
    UTF8String text;

    text = pEntry->refAction.text.GetUTF16();

    pItem = gtk_menu_item_new_with_label((gchar *)text.GetC_Str());
    gtk_menu_shell_append(GTK_MENU_SHELL(THIS->pSubMenu), pItem);
    gtk_widget_show(pItem); //default is show

    g_signal_connect(G_OBJECT(pItem), "activate", G_CALLBACK(OnMenuItemActivatedHandler), pEntry);
}

void CMenu::DestructOS()
{
    MemFree(this->pOSHandle);
}
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
#include <ACStdLib/UI/Views/CTreeView.h>
//Global
#include <gtk/gtk.h>
//Local
#include "../Gtk.h"
#include <ACStdLib/UI/Controllers/ATreeController.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;
//Definitions
#define THIS ((GtkWidget *)(this->systemHandle))

//Local functions
/*
static HTREEITEM InsertItemAtFront(HWND hWnd, HTREEITEM hItem, void *node, const String &refText)
{
    TVINSERTSTRUCTW tvis;
    UTF16String textUTF16;

    textUTF16 = refText.GetUTF16();

    tvis.hParent = hItem;
    tvis.hInsertAfter = TVI_FIRST;

    //item
    tvis.item.mask = TVIF_TEXT | TVIF_PARAM;
    tvis.item.pszText = (LPWSTR)textUTF16.GetC_Str();
    tvis.item.cchTextMax = textUTF16.GetLength();
    tvis.item.lParam = (LPARAM)node;

    return (HTREEITEM)SendMessage(hWnd, TVM_INSERTITEMW, 0, (LPARAM)&tvis);
}
*/

static void AddNodes(GtkTreeStore *store, GtkTreeIter *nodeIter, void *pNode, ATreeController &refController)
{
    uint32 nChildren, i;
    void *pChildNode;
    GtkTreeIter childIter;
    UTF8String text;

    nChildren = refController.GetNumberOfChildren(pNode);
    for(i = 0; i < nChildren; i++)
    {
        pChildNode = refController.GetChild(pNode, i);
        //hChild = InsertItemAtFront(hWnd, hItem, pChildNode, refController.GetText(pChildNode));

        text = refController.GetText(pChildNode).GetUTF16();
        gtk_tree_store_append(store, &childIter, nodeIter);
        gtk_tree_store_set(store, &childIter, 0, text.GetC_Str(), -1);

        //AddNodes(hWnd, hChild, pChildNode, refController);
    }
}

//Eventhandlers
void CTreeView::OnModelChanged()
{
    gtk_tree_view_set_model(GTK_TREE_VIEW(THIS), nullptr);

    if(this->pController)
    {
        GtkTreeStore *store;

        store = gtk_tree_store_new(1, G_TYPE_STRING);

        AddNodes(store, nullptr, nullptr, *this->pController);

        gtk_tree_view_set_model(GTK_TREE_VIEW(THIS), GTK_TREE_MODEL(store));
    }
}

//Private methods
void CTreeView::CreateOSWindow()
{
    this->systemHandle = gtk_tree_view_new();
    gtk_widget_set_vexpand(THIS, TRUE);

	ADD_SELF_TO_PARENT(THIS);

    //append the standard column
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    renderer = gtk_cell_renderer_text_new();

    column = gtk_tree_view_column_new_with_attributes("", renderer, "text", 0, nullptr);
    gtk_tree_view_column_set_sort_column_id(column, 0);
    gtk_tree_view_append_column(GTK_TREE_VIEW(THIS), column);
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(THIS), false); //make it invisible


    gtk_widget_show_all(THIS); //default is show
}
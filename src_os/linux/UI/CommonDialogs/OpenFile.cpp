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
//Corresponding header
#include <ACStdLib/UI/CommonDialogs.h>
//Global
#include <gtk/gtk.h>
//Local
#include "../Gtk.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Namespace functions
CPath CommonDialogs::OpenFile(const Window &refParentWnd, const CString &refTitle, const CLinkedList<CTuple<CString, CString>> &refFilters)
{
    gint result;
    GtkWidget *pDialog;
    CUTF8String titleUTF8;
    CString fileName;

    titleUTF8 = refTitle.GetUTF16();
    pDialog = gtk_file_chooser_dialog_new((gchar *)titleUTF8.GetC_Str(), GTK_WINDOW(INTERNAL_WIDGET_CONTAINER(&refParentWnd)), GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, nullptr);

    result = gtk_dialog_run(GTK_DIALOG(pDialog));
    if(result == GTK_RESPONSE_ACCEPT)
    {
        char *pFileName;

        pFileName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(pDialog));
        fileName = CUTF8String(pFileName);

        g_free(pFileName);
    }

    gtk_widget_destroy(pDialog);

    return fileName;
}
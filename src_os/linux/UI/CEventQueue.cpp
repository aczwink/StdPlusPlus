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
#include "../../../headers/UI/CEventQueue.h"
//Global
#include <gtk/gtk.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Public functions
void CEventQueue::PostQuitEvent(int32 exitCode)
{
    gtk_main_quit();
}

int32 CEventQueue::ProcessEvents()
{
    gtk_main();

    return EXIT_SUCCESS;
}

bool CEventQueue::ProcessPendingEvents(int32 &refExitCode)
{
    bool result;

    result = true;

    while(gtk_events_pending())
        result = gtk_main_iteration() == 0;

    return result;
}
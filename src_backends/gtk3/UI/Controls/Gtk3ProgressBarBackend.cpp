/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
#include "Gtk3ProgressBarBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
Gtk3ProgressBarBackend::Gtk3ProgressBarBackend(UIBackend &uiBackend, ProgressBar &progressBar)
    : ProgressBarBackend(uiBackend), WidgetBackend(uiBackend), Gtk3WidgetBackend(uiBackend, gtk_progress_bar_new()),
    progressBar(progressBar)
{

}

//Public methods
const Widget &Gtk3ProgressBarBackend::GetWidget() const
{
    return this->progressBar;
}

void Gtk3ProgressBarBackend::SetProgress(float64 progress)
{
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(this->GetGtkWidget()), progress);
}


//NOT IMPLEMENTED
void Gtk3ProgressBarBackend::Repaint() {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
//NOT IMPLEMENTED
/*
 * Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
#include "Gtk3SliderBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
Gtk3SliderBackend::Gtk3SliderBackend(UIBackend &uiBackend, Slider& slider)
    : SliderBackend(uiBackend), WidgetBackend(uiBackend),
    Gtk3WidgetBackend(uiBackend, gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 1)),
    slider(slider)
{
}

//Public methods
Widget &Gtk3SliderBackend::GetWidget()
{
    return this->slider;
}

const Widget &Gtk3SliderBackend::GetWidget() const
{
    return this->slider;
}

void Gtk3SliderBackend::SetPosition(uint32 pos)
{
    gtk_range_set_value(GTK_RANGE(this->GetGtkWidget()), pos);
}

void Gtk3SliderBackend::SetRange(uint32 min, uint32 max)
{
    gtk_range_set_range(GTK_RANGE(this->GetGtkWidget()), min, max);
}

//NOT IMPLEMENTED
void _stdxx_::Gtk3SliderBackend::Repaint() {
    NOT_IMPLEMENTED_ERROR; //TODO: IMPLEMENT ME
}

void _stdxx_::Gtk3SliderBackend::SetEditable(bool enable) const {
    NOT_IMPLEMENTED_ERROR; //TODO: IMPLEMENT ME
}
//END OF NOT IMPLEMENTED
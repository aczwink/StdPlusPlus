/*
 * Copyright (c) 2023 Amir Czwink (amir130@hotmail.de)
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
#include "Gtk3MenuBarBackend.hpp"
//Local
#include <Std++/UI/Menu/Menu.hpp>
#include "Gtk3MenuBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX::UI;

void Gtk3MenuBarBackend::AppendMenu(StdXX::UI::Menu* menu)
{
	const auto* menuBackend = dynamic_cast<const Gtk3MenuBackend *>(menu->_GetBackend());

	gtk_menu_shell_append (GTK_MENU_SHELL (this->GetGtkWidget()), menuBackend->MenuItemGtkWidget());
}

const Widget& Gtk3MenuBarBackend::GetWidget() const
{
	return this->menuBar;
}

void Gtk3MenuBarBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
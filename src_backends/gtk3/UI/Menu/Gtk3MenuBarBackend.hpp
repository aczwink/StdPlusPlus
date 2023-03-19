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
//Local
#include <Std++/UI/Menu/MenuBar.hpp>
#include "../Gtk3WidgetBackend.hpp"

namespace _stdxx_
{
	class Gtk3MenuBarBackend : public MenuBarBackend, public Gtk3WidgetBackend
	{
	public:
		//Constructor
		inline Gtk3MenuBarBackend(StdXX::UIBackend& uiBackend, StdXX::UI::MenuBar& menuBar) : Gtk3WidgetBackend(uiBackend, gtk_menu_bar_new()),
			WidgetBackend(uiBackend), menuBar(menuBar)
		{
			gtk_widget_show_all(this->GetGtkWidget());
		}

		//Methods
		void AppendMenu(StdXX::UI::Menu *menu) override;
		const StdXX::UI::Widget &GetWidget() const override;
		void Repaint() override;

	private:
		//State
		StdXX::UI::MenuBar& menuBar;
	};
}
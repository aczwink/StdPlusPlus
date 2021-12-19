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
//Local
#include <Std++/_Backends/UI/MenuBackend.hpp>
#include <Std++/_Backends/UI/UIBackend.hpp>
#include "../Gtk3WidgetBackend.hpp"

namespace _stdxx_
{
    class Gtk3MenuBackend : public MenuBackend
    {
    public:
        //Constructor
        Gtk3MenuBackend(StdXX::UIBackend& uiBackend, StdXX::UI::Menu& menu);

        //Methods
        void AppendEntry(const StdXX::UI::ActionEntry &actionEntry) override;
        void SetText(const StdXX::String &text) override;
        void ShowPopup() override;

        //NOT IMPLEMENTED
        void AppendSeperator() override;
        void AppendSubMenu(StdXX::UI::Menu *subMenu) override;
        //NOT IMPLEMENTED

    private:
        //Members
        GtkWidget* gtkWidget;
    };
}
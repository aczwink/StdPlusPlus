/*
 * Copyright (c) 2018-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/UI/Containers/GroupBox.hpp>
#include "Gtk3WidgetBackend.hpp"

namespace _stdxx_
{
    class Gtk3GroupBoxBackend : public GroupBoxBackend, public Gtk3WidgetBackend
    {
    public:
        //Constructor
        Gtk3GroupBoxBackend(StdXX::UIBackend& uiBackend, StdXX::UI::GroupBox& groupBox);

        //Methods
		void AddChild(StdXX::UI::Widget *widget) override;
        WidgetContainerBackend *CreateContentAreaBackend(StdXX::UI::CompositeWidget &widget) override;
        StdXX::Math::RectD GetContentAreaBounds() const override;
        const StdXX::UI::Widget &GetWidget() const override;
        void SetTitle(const StdXX::String &title) override;

        //NOT IMPLEMENTED
        void Repaint() override;
        //END OF NOT IMPLEMENTED

    private:
        //Members
        StdXX::UI::GroupBox& groupBox;
    };
}
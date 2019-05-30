/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/_Backends/UI/WidgetContainerBackend.hpp>
#include <Std++/UI/Containers/CompositeWidget.hpp>
#include "_RedirectGtkContainer.h"
#include "Gtk3WidgetBackend.hpp"

namespace _stdxx_
{
    class Gtk3RedirectContainer : public WidgetContainerBackend, public Gtk3WidgetBackend
    {
    public:
        //Constructor
        Gtk3RedirectContainer(StdXX::UIBackend& uiBackend, StdXX::UI::CompositeWidget &widget);

        //Methods
		void AddChild(StdXX::UI::Widget *widget) override;
	    StdXX::Math::SizeD GetSizeHint() const override;
        StdXX::UI::Widget &GetWidget() override;
        const StdXX::UI::Widget &GetWidget() const override;

        //NOT IMPLEMENTED
        void IgnoreEvent() override;
        void Repaint() override;
        void SetEditable(bool enable) const override;
        //NOT IMPLEMENTED

    private:
        //Members
        StdXX::UI::CompositeWidget &widget;
    };
}
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
//Local
#include <Std++/UI/Controls/Slider.hpp>
#include "Gtk3WidgetBackend.hpp"

namespace _stdxx_
{
    class Gtk3SliderBackend : public SliderBackend, public Gtk3WidgetBackend
    {
    public:
        //Constructor
        Gtk3SliderBackend(StdXX::UIBackend& uiBackend, StdXX::UI::Slider& slider);

        //Methods
        StdXX::UI::Widget &GetWidget() override;
        const StdXX::UI::Widget &GetWidget() const override;
        void SetPosition(uint32 pos) override;
        void SetRange(uint32 min, uint32 max) override;

        //NOT IMPLEMENTED
        void Repaint() override;
        void SetEditable(bool enable) const override;
        //END OF NOT IMPLEMENTED

    private:
        //Members
        StdXX::UI::Slider& slider;
    };
}
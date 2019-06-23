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
#pragma once
//Global
#include <gtk/gtk.h>
//Local
#include <Std++/_Backends/UI/WidgetBackend.hpp>

namespace _stdxx_
{
    class Gtk3WidgetBackend : virtual public WidgetBackend
    {
    public:
        //Constructor
        inline Gtk3WidgetBackend(StdXX::UIBackend& uiBackend, GtkWidget *gtkWidget) : WidgetBackend(uiBackend), gtkWidget(gtkWidget)
        {
        }

        //Methods
        StdXX::Math::SizeD GetSizeHint() const override;
	    void IgnoreEvent() override;
	    void SetBounds(const StdXX::Math::RectD &bounds) override;
        void SetEnabled(bool enable) override;
        void SetHint(const StdXX::String &text) override;
        void Show(bool visible) override;

		//Inline
		inline GtkWidget *GetGtkWidget()
		{
			return this->gtkWidget;
		}

	    inline const GtkWidget *GetGtkWidget() const
	    {
		    return this->gtkWidget;
	    }

    private:
        //Members
        GtkWidget *gtkWidget;
    };
}
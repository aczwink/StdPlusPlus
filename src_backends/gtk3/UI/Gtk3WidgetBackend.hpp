/*
 * Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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
        Gtk3WidgetBackend(StdXX::UIBackend& uiBackend, GtkWidget *gtkWidget);

        //Methods
        StdXX::Math::RectD GetNextAssignmentBoundsInGtkCoords() const;
        StdXX::Math::SizeD GetSizeHint() const override;
	    void IgnoreEvent() override;
		void OnSetAllocation(const GtkAllocation& allocation);
	    void SetBounds(const StdXX::Math::RectD &bounds) override;
        void SetEnabled(bool enable) override;
        void SetHint(const StdXX::String &text) override;
        void Show(bool visible) override;

        //Functions
        static inline Gtk3WidgetBackend* Gtk3WidgetBackendFromGtkWidget(GtkWidget* gtkWidget)
        {
            return static_cast<Gtk3WidgetBackend *>(g_object_get_data(G_OBJECT(gtkWidget), u8"Std++"));
        }

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
		StdXX::Math::RectD nextBounds;
    };
}
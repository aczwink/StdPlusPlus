/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/_Backends/UI/DialogBackend.hpp>
#include <Std++/UI/Windows/Dialog.hpp>
#include "Gtk3WindowBackend.hpp"

namespace _stdxx_
{
	class Gtk3DialogBackend : public virtual DialogBackend, public virtual Gtk3WindowBackend
	{
	public:
		//Constructor
		inline Gtk3DialogBackend(StdXX::UIBackend& uiBackend, StdXX::UI::Dialog& dialog)
			: Gtk3WindowBackend(uiBackend, dialog), Gtk3WidgetBackend(uiBackend, gtk_dialog_new_with_buttons("", nullptr, (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_USE_HEADER_BAR), nullptr, nullptr)),
			DialogBackend(uiBackend), WidgetBackend(uiBackend), WidgetContainerBackend(uiBackend), WindowBackend(uiBackend),
			dialog(dialog)
		{
			//gtk_window_set_modal(GTK_WINDOW(this->GetGtkWidget()), TRUE);
		}

		//Destructor
		~Gtk3DialogBackend();

		//Methods
		void AddChild(StdXX::UI::Widget *widget) override;
		bool Run(StdXX::UI::Property<bool>& isValid) override;

	private:
		//State
		StdXX::UI::Dialog& dialog;
	};
}
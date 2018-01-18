/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Global
#include <gtk/gtk.h>
//Local
#include <ACStdLib/_Backends/WindowBackend.hpp>
#include <ACStdLib/UI/Widget.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

namespace _ACStdLib_internal
{
	class GtkWindowBackend : public WindowBackend
	{
	public:
		//Constructor
		GtkWindowBackend(_ACStdLib_internal::WindowBackendType type, Widget *widget);

		//Destructor
		~GtkWindowBackend();

		//Methods
		ACStdLib::Size GetSize() const;
		Path SelectExistingDirectory(const String &title, const Function<bool(ACStdLib::Path &)> callback) const;
		void SetBounds(const ACStdLib::Rect &area);
		void Show(bool visible);
		void ShowInformationBox(const String &title, const String &message) const;

	private:
		//Members
		GtkWidget *gtkWidget; //the widget, window whatever itself
		GtkWidget *childAreaWidget; //its child area
	};
}